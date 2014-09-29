#! /usr/bin/tclsh 

proc sendRequest {f str} {
    puts $f "$str"
    flush $f
    #puts stdout "$str"
}

proc printReply {f} {
    if {[gets $f reply] >= 0} {
        puts "$reply"
        flush stdout
    }
}

set f [open {/dev/ttyUSB0} {RDWR}]
fconfigure $f -blocking 1 -buffering line  -encoding binary  -mode 57600,n,8,1 -timeout 10000
fileevent $f readable [list printReply $f]

sendRequest $f {out 0 on}
after 60000
sendRequest $f {out 0 off; wait 1000;}

sendRequest $f {out 0 blink 50}


close $f

