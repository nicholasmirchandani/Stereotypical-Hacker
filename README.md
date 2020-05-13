# Stereotypical-Hacker

## Overview
Stereotypical Hacker – Mainframe Access Token is a project we’ve developed as a team to unleash the immersive powers of networking to keep players engaged by playing with each other.  We wanted to push ourselves to the limit and create something truly worthwhile in the form of a gamified hacking experience, while also learning skills required to incorporate various different ideas seamlessly into a final product.  Not only does this project implement various networking protocols simultaneously to create an enjoyable game, but it also does so while allowing us to constructively teach players simple cybersecurity concepts, as they would seem from the point of view of an experienced stereotypical hacker.  We believe that by introducing players to these concepts, they’ll take measures to secure their own data from real-world hackers, especially after seeing how easy things seem to the hacker.  However, above all, our goal is to grant players an unforgettable experience to share with their friends that’ll show them what fun truly is.  We believe that we accomplished our goal, but the only way to find out is to try it for yourself.

## Build Instructions
For client: g++ client.cpp -lpthread -o client
For server: g++ server.cpp Player.cpp VirtualServer.cpp -lpthread -O0 -o server
