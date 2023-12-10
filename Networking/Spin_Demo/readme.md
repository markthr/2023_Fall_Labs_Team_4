#### State Codes

| Code | Name |
| --- | --- |
| 0 | Startup |
| 1 | Rest|
| 2 | Stop |
| 3 | Spin |

* `Startup` is the initial state when the device is powered on during which any needed configuration is performed
* `Rest` is an idle state where the bot awaits a command
* `Stop` is a stop that briefly brakes before transitioning t stop
* `Spin` has the bot spin 360 degrees