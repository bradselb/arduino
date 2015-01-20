#! /usr/bin/tclsh 

proc sendRequest {f str} {
    flush stderr
    flush stdout
    flush $f
    puts $f "$str"
    flush $f
#    puts stdout {}
}

proc printReply {f} {
#    puts stderr {.}
    global done
    if {[gets $f reply] >= 0} {
        puts "$reply"
        flush stdout
        if {0 == [string compare -nocase -len 2 $reply "OK"]} {
            set done {success}
        }
    }
}

set done 0
after 2000 [list set done {timeout}]

if { [catch {open /dev/ttyUSB0 r+} f] } {
    puts stderr "failed to open the serial port"
    exit 1
}

set ready 0
fconfigure $f  -mode 57600,n,8,1 -timeout 1000 -blocking 1 -buffering full  -translation binary
fileevent $f readable [list printReply $f]
fileevent $f writable [list set ready 1]

vwait ready
#puts stderr "ready!"

foreach item $argv {
    sendRequest $f "$item"
}

# send some un-recognized command to get back "OK"
#sendRequest $f "junk"


vwait done
puts "$done"
close $f

