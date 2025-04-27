Computer system zarchim 2 - Sukhbat

## make

- make clean
- make all

## play

**1 pc ndeer**

Terminal 1 / host - toglogch 1/ - `./server 5000`
Terminal 2 / client - toglogch 2/ - `./client 127.0.0.1 8080`

**2 pc ba 1 network**

`LOCALIP` = local ip haygaa olno.
pc1 / host - toglogch 1/ - `./server 5000`
pc2 / client - toglogch 2/ - `./client LOCALIP 5000`

**2 pc ba 2 network**
pc1 / host - toglogch 1/ - `./server 5000`
openssh ashiglan tunnel uusgne `SERVEO_ADDRESS` = `ssh -R 0:localhost:5000 serveo.net`
pc2 / client - toglogch 2/ - `./client serveo.net SERVEO_ADDRESS`