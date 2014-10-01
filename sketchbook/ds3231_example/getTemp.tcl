#! /usr/bin/tclsh 

after 10000 [list set state timeout]

proc sendRequest {f} {
#    puts  $f {}
    puts $f "show t"
    flush $f
#    puts stdout "request sent"
}

proc printReply {f} {
    global state
    if {[gets $f reply] >= 0} {
        puts -nonewline "$reply "
        flush stdout
    }
}

set f [open {/dev/ttyACM0} {RDWR}]
fconfigure $f -blocking 1 -buffering line  -encoding binary  -mode 57600,n,8,1 -timeout 10000
# fileevent $f writable [list sendRequest $f]
# fileevent $f readable [list printReply $f]

after 2000
sendRequest $f
printReply $f
printReply $f
puts stdout {}

# vwait state
# puts stdout "$state"

close $f
