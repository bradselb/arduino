#! /usr/bin/tclsh 

proc sendRequest {f str} {
    puts $f "$str"
    flush $f
    #puts stdout "$str"
}

proc printReply {f} {
    global done
    if {[gets $f reply] >= 0} {
        if {0 == [string compare -nocase -len 2 $reply "OK"]} {
            set done {success}
        } else {
            puts "$reply"
            flush stdout
        }
    }
}

set done 0
after 1000 [list set done {timeout}]

set f [open {/dev/ttyUSB0} {RDWR}]
fconfigure $f -blocking 1 -buffering line  -encoding binary  -mode 57600,n,8,1 -timeout 10000
fileevent $f readable [list printReply $f]

foreach item $argv {
    sendRequest $f "show $item"
}

# send some un-recognized command to get back "OK"
sendRequest $f "junk"


vwait done

close $f

