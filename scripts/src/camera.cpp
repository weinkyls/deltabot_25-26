#include camera.h

enum class PixelFormat {
    NV12,
    YUYV,
    MJPG,
    RGB24,
    BGR24,
    ANY
};

struct FrameSize {
    int width = 0;
    int height = 0;
};

struct FrameRate {
    int num = 0;   // e.g. 30
    int den = 1;   // e.g. 1  (so 30/1)
};


struct CameraSpec {
    std::vector<PixelFormat> preferred_formats;        // ordered preference
    std::vector<FrameSize> preferred_sizes;            // ordered preference
    std::vector<int> preferred_fps;                    // ordered preference (fps integers)
    int required_camera_count = 2;                     // usually 2
    bool require_distinct_devices = true;
};


struct Mode {
    uint32_t v4l2_fourcc = 0;      // e.g. V4L2_PIX_FMT_NV12
    FrameSize size;
    FrameRate framerate;          // optional; may be unknown if not enumerated
};


struct DeviceInfo {
    std::string devnode;          // "/dev/video2"
    std::string driver;           // from v4l2_capability.driver
    std::string card;             // from v4l2_capability.card
    std::string bus_info;         // from v4l2_capability.bus_info

    bool supports_capture = false;    // VIDEO_CAPTURE or MPLANE
    bool supports_streaming = false;  // STREAMING

    std::vector<Mode> modes;          // all discovered modes (format+size(+fps))
};

struct SelectedCamera {
    DeviceInfo device;
    Mode chosen_mode;

    // built later:
    std::string gst_pipeline;
};

class V4L2Enumerator {
public:
    struct Options {
        int max_video_nodes = 64;           // scan /dev/video0..63
        bool enumerate_frame_intervals = true; // whether to enumerate FPS list
    };

    explicit V4L2Enumerator(Options opt = {});
    std::vector<DeviceInfo> enumerate() const;

private:
    Options opt_;

    // internal helpers (conceptual)
    static bool is_video_node_present(const std::string& path);
    static bool query_device_caps(int fd, DeviceInfo& out);
    static void enumerate_formats_sizes_fps(int fd, DeviceInfo& out, bool enumerate_fps);
};

class ModeSelector {
public:
    struct Score {
        int device_score = 0;
        int mode_score = 0;
        // lower is better or higher is better; pick one convention and stick to it
    };

    struct Options {
        bool prefer_lower_or_equal_fps = true;      // if exact fps not available
        bool prefer_lower_or_equal_size = true;     // if exact size not available
    };

    explicit ModeSelector(Options opt = {});
    std::vector<SelectedCamera> select(
        const std::vector<DeviceInfo>& devices,
        const CameraSpec& spec
    ) const;

private:
    Options opt_;

    // internal helpers (conceptual)
    static bool device_is_candidate(const DeviceInfo& d);
    static int format_rank(uint32_t fourcc, const CameraSpec& spec);
    static int size_rank(const FrameSize& s, const CameraSpec& spec, bool prefer_lower);
    static int fps_rank(const FrameRate& fr, const CameraSpec& spec, bool prefer_lower);

    static bool same_physical_device(const DeviceInfo& a, const DeviceInfo& b); // optional heuristic
};

class GstPipelineBuilder {
public:
    enum class Output {
        BGR,   // for OpenCV
        NV12,  // for performance / later
        RAW    // rarely, but possible
    };

    struct Options {
        Output output = Output::BGR;
        bool add_videoconvert = true;      // needed for BGR output usually
        bool force_io_mode = false;
        int io_mode = 0;                   // only used if force_io_mode=true
    };

    explicit GstPipelineBuilder(Options opt = {});
    std::string build(const std::string& devnode, const Mode& mode) const;

private:
    Options opt_;

    static std::string fourcc_to_gst_caps(uint32_t v4l2_fourcc); // e.g. NV12 -> "NV12"
};

struct FrameMeta {
    uint64_t frame_id = 0;        // your own counter
    uint64_t monotonic_ns = 0;    // time you received it (steady clock)
    // Optional later:
    // uint64_t v4l2_timestamp_ns;
};

struct Frame {
    cv::Mat image;
    FrameMeta meta;
};


class CaptureCameraFeed {
public:
    explicit CaptureCameraFeed(std::string gst_pipeline);
    ~CaptureCameraFeed();

    void start();
    void stop();

    bool isRunning() const;

    // latest frame (copy-out)
    Frame getLatestFrame() const;

private:
    void runLoop();

    std::string pipeline_;
    mutable std::mutex mtx_;
    std::atomic<bool> running_{false};

    cv::VideoCapture cap_;

    Frame latest_;
    std::thread worker_;
};


struct FramePair {
    Frame a;
    Frame b;
    int64_t delta_ns = 0; // a.meta.monotonic_ns - b.meta.monotonic_ns
};

class DualCameraManager {
public:
    struct Options {
        int64_t max_pair_delta_ns = 15'000'000; // e.g. 15ms tolerance
    };

    DualCameraManager(std::unique_ptr<CaptureCameraFeed> camA,
                      std::unique_ptr<CaptureCameraFeed> camB,
                      Options opt = {});

    void start();
    void stop();

    // returns true if a valid pair is produced
    bool tryGetPairedFrames(FramePair& out);

private:
    Options opt_;
    std::unique_ptr<CaptureCameraFeed> camA_;
    std::unique_ptr<CaptureCameraFeed> camB_;
};

class CameraSystem {
public:
    explicit CameraSystem(CameraSpec spec);

    bool initialize();   // enumerate + select + build pipelines
    void start();
    void stop();

    bool tryGetFramePair(FramePair& out);

private:
    CameraSpec spec_;

    std::vector<DeviceInfo> devices_;
    std::vector<SelectedCamera> selected_;

    std::unique_ptr<DualCameraManager> dual_;
};
