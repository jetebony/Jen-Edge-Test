#CHANGELOG

## v0.5.5

 * Update circle.yml file to simplify the build depending on the github branch

## v0.5.4

 * removed slack token from yml file
 
## v0.5.3

 * Do not call MQTT loop_stop for unintentional disconnect

## v0.5.2

 * Changed 'clean session' to default false
 * fixed string error in logger
 * removed leading '-' from MQTT client name

## v0.5.1

 * Adjusted MQTT keealive value

## v0.5.0

 * bumped version

## v0.4.4

 * fixed memory leak - pointer out of scope

## v0.4.3

 * Fixed MQTT issues and added unittests to confirm fixes. 
 * Added `writeSyslogEntry` to logger for mosquitto to log to.

## v0.4.2

 * remove extra mqtt connects from subscribe, unsubscribe and send functions

## v0.4.1

 * disable retained MQTT messages & set QOS to 2
