#! /usr/bin/tclsh 

after 5000 [list set state timeout]

proc sendRequest {f msg}  {
    puts $f "$msg"
    flush $f
}

proc printReply {f} {
    global state
    if {[gets $f reply] >= 0} {
        puts -nonewline "$reply "
        flush stdout
        set state {done}
    }
}

set f [open {/dev/ttyUSB0} {RDWR}]
fconfigure $f -blocking 1 -buffering line  -encoding binary  -mode 57600,n,8,1; # -timeout 1000
# fileevent $f writable [list sendRequest $f]
fileevent $f readable [list printReply $f]

sendRequest $f "get temp"

vwait state
puts stdout {}

close $f
