#! /usr/bin/tclsh 

after 10000 [list set state timeout]

proc sendRequest {f msg}  {
#    puts  $f {}
    puts $f "$msg"
    flush $f
#    puts stdout "request sent"
}

proc printReply {f} {
    if {[gets $f reply] >= 0} {
        puts -nonewline "$reply "
        flush stdout
    }
}

set f [open {/dev/ttyACM0} {RDWR}]
fconfigure $f -blocking 1 -buffering line  -encoding binary  -mode 57600,n,8,1; # -timeout 1000
# fileevent $f writable [list sendRequest $f]
fileevent $f readable [list printReply $f]

after 2000
sendRequest $f {show temp}

vwait state
puts stdout {}

close $f
