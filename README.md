# usleep-windows
High accuracy sleep function for Windows

Works by setting the Windows Timer Resolution to 0.5 milliseconds and then creating a WaitableTimer with the `CREATE_WAITABLE_TIMER_HIGH_RESOLUTION` flag.

```
// Sleeps for at least the given microsecond time period.
//
// The accuracy is usually only on the millisecond scale.
// The highest resolution maxes out at 0.5 milliseconds.
// See benchmarks below.
//
// Example benchmark results:
// usleep(16000) -> sleeps for 16.32  milliseconds on average.
// usleep( 8000) -> sleeps for  8.27  milliseconds on average.
// usleep( 2000) -> sleeps for  2.33  milliseconds on average.
// usleep( 1000) -> sleeps for  1.36  milliseconds on average.
// usleep(  500) -> sleeps for  0.97  milliseconds on average.
// usleep(  200) -> sleeps for  0.501 milliseconds on average.
// usleep(   20) -> sleeps for  0.496 milliseconds on average.
// usleep(    5) -> sleeps for  0.488 milliseconds on average.
// usleep(    1) -> sleeps for  0.387 milliseconds on average.
void usleep(long long microseconds);
```


Based on
* https://randomascii.wordpress.com/2020/10/04/windows-timer-resolution-the-great-rule-change/
* https://gist.github.com/GoaLitiuM/aff9fbfa4294fa6c1680
* https://github.com/randomascii/blogstuff/blob/main/timer_interval/waitable_timer.cpp

