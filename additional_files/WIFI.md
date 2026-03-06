# WIFI setup

nmcli connection add type wifi con-name "Ralink MT7601U" ifname wlx28f36620f3c7 ssid xzyID
nmcli con modify "Ralink MT7601U" wifi-sec.key-mgmt wpa-psk
nmcli con modify "Ralink MT7601U" wifi-sec.psk supersecretKEY
nmcli con up  "Ralink MT7601U"

