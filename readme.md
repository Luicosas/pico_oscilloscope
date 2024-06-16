# To allow access to /dev/ttyACM0 on endeavor OS 
Running `ls -l /dev/ttyACM0` shows `crw-rw---- 1 root uucp 166, 0 Jun 15 18:11 /dev/ttyACM0`
so we add the user to uucp with `sudo gpasswd -a user uucp` but for some reason we need to run
`newgrp uucp` for the permissions to catch on. 

Running `cat /dev/ttyACM0` should no longer get permissions denied error.
