
Time cmd 1-9 resp +100
Get Time
req:  id=123&cmd=1#
resp: id=123&cmd=101&rst=0&date=2019-12-23&time=12:23:34&week=3

Update Time
req:
Id=123&cmd=2&date=2018-1-29&time=20:58:34&week=1#
Id=123&cmd=2&date=2018-1-26#
Id=123&cmd=2&time=20:55:34#
Id=123&cmd=2&week=3#

resp: Id=123&cmd=102&result=0


Timer cmd: 
Create Timer:
req:
Id=123&cmd=11&tId=0&rId=2&sdays=127&stime=12:23:34&etime=23:34:45&interm=0-0#
resp: Id=123&cmd=111&tId=1&result=0

Get Timer:
req:  
Id=123&cmd=12&tId=1#
resp: Id=123&cmd=113&tId=1&rId=1&sdays=5&stime=12:23:34&etime=23:34:45&interm=12-45



Update Timer:
req: 
Id=123&cmd=13&tId=1&rid=1&sdays=5&stime=22:22:22&etime=22:22:33#
Id=123&cmd=13&tId=1&stime=22:22:22&etime=22:22:33#

Id=123&cmd=13&tId=1&rId=1&sdays=127#

resp: Id=123&cmd=112&tId=1&result=0#

disable Timer:
req:
Id=123&cmd=14&tId=0#



Show all Timer:
Id=123&cmd=19&date=2019-12-23&time=12:23:34&week=3#
Id=123&cmd=19&time=12:23:34&week=3#
Id=123&cmd=19&week=3#
Id=123&cmd=19#


Signal
Show all Signal:
Id=123&cmd=29#

reset all signal:
Id=123&cmd=23#








