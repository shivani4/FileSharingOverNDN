------------------------------------------
Description of the working of the program:
------------------------------------------
The program is based on the NDN model and is built on the NDN API. The setup was done as per the NDN documentation. All the prerequisites were satisfied.

./waf inside the fshare folder will compile the file.

waf script is present inside the fshare folder as it contains the directory structure.
Please untar the folder and extract the contents.It contains C++ source code, folder for wafscript and one readme.

Below are details regarding the working of the program:
----------------------
Setting up the Network
----------------------
Below are the individual commands to setup the network:
Node A consists of two files as nodea-_cs557_test1_1.jpg and 

nodea-_cs557_test1_1.jpg


Node A

nfdc create tcp4://nodeb.proj2.csucs557.isi.deterlab.net




./fshare -p /cs557/test1/1.jpg /cs557/test2/1.jpg


 //Publishes the file

Node B
------------------------------------

nfdc create tcp4://nodea.proj2.csucs557.isi.deterlab.net

nfdc create tcp4://nodec.proj2.csucs557.isi.deterlab.net

nfdc create tcp4://noded.proj2.csucs557.isi.deterlab.net

nfdc create tcp4://nodee.proj2.csucs557.isi.deterlab.net



nfdc register /cs557/test1/ tcp4://nodea.proj2.csucs557.isi.deterlab.net

nfdc register /cs557/test2/ tcp4://nodea.proj2.csucs557.isi.deterlab.net




Node C
------------------------------------
nfdc create tcp4://nodeb.proj2.csucs557.isi.deterlab.net
nfdc register /cs557/test1/ tcp4://nodeb.proj2.csucs557.isi.deterlab.net

nfdc register /cs557/test2/ tcp4://nodeb.proj2.csucs557.isi.deterlab.net




./fshare -d /cs557/test1/1.jpg /cs557/test2/1.jpg //Will download the file

Node D
------------------------------------

nfdc create tcp4://nodeb.proj2.csucs557.isi.deterlab.net

nfdc register /cs557/test1/ tcp4://nodeb.proj2.csucs557.isi.deterlab.net
nfdc register /cs557/test2/ tcp4://nodeb.proj2.csucs557.isi.deterlab.net
./fshare -d /cs557/test1/1.jpg /cs557/test2/1.jpg //Will download the file

Node E
------------------------------------

nfdc create tcp4://nodeb.proj2.csucs557.isi.deterlab.net
nfdc register /cs557/test1/ tcp4://nodeb.proj2.csucs557.isi.deterlab.net
nfdc register /cs557/test2/ tcp4://nodeb.proj2.csucs557.isi.deterlab.net
./fshare -d /cs557/test1/1.jpg /cs557/test2/1.jpg //Will download the file

After running these commands, we will have individual files for each node for each prefix.


----------------------------------------------------------------------------------
Options available:
----------------------------------------------------------------------------------
Below are the options for running the program:
It can run with two options or both

./fshare -p [[finlenames....]]

./fshare -d [[finlenames....]]

./fshare -d [[finlenames....]]  -p [[finlenames....]]

The prefixes will need to be registered. 

This will result in the creation of the files with the prefixes. Below is the result of the runs on these files.These runs were in miliseconds as per the time taken n UNIX. 

Results:(File size 24kb)
Run1: Node C :3054  Node D:1156 Node E: 1152
Run2: Node C: 1078 sec  Node D:1004 sec Node E: 1056sec
Run3: Node C: 1072  Node D:1066 Node E: 1053
Run4: Node C: 1032  Node D:1003 sec Node E: 1001
Run5: Node C: 950  Node D:978 sec Node E: 982

The data is almost same in the subsequent runs.
It can be clearly seen from the above results that the first run took large time for C as it had taken the data from A. For nodes D and E the data was readily available from C as it was in the same folder. 
For the next run, the data was available for C as well, so it downloaded from D or E. In the subsequent runs, the time decreases more as the router itself contains the requested data ans is readily available in the cache.

------------------------------------------------------
Behaviour under certain scenarios
------------------------------------------------------
Below are the various scenarios and how the code would behave for each one of them:
The program sends files in 32 segment size. It first takes the complete size and then sends request for multiple segments until the max is reached. 
The file will also generate a log file which contains the begin and end time of the file.
It works for images, pdfs as well


References:
Stackoverflow.com
To install NDN- http://named-data.net/doc/NFD/current/INSTALL.html#building-from-source
