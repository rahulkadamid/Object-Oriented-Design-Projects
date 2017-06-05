Command line args:
-------------------
TestExec.exe ../CodePublisher/ *.h *.cpp  /r ../Repository/index.htm

"../Repository/index.htm" -> Give path to any htm file to be able to open that htm file in browser

eg:
TestExec.exe ../CodePublisher/ *.h *.cpp  /r ../Repository/main.cpp.htm

Repository folder:
------------------
Code publisher saves all the input published files into this folder.
The folder also contains "index.htm" which contains hyperlinked list of all files published by Code publisher.

theme.css:
----------
CSS stylesheet used in the head tag of each html file

myscripts.js:
------------
Javascript file also used in the head tag of each html file used for expanding and collapsing code bodies with scopes.

plusik.gif:
-----------
Its an Image of plus sign "+" click on this image to expand and collapse code body scopes.