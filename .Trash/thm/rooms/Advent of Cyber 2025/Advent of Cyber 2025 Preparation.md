---
title: Advent of Cyber 2025  — TryHackMe
tags:
  - cybersecurity
  - thm
  - advent_of_cyber
slug: Advent of Cyber Prep Track
summary: Get ready for the Advent of Cyber 2025 with the "Advent of Cyber Prep Track", a series of warm-up tasks aimed to get beginners ready for this year's event.
---
11-23-2025
17:54
## Introduction

## Content
"This name keeps coming up" - `King Malhare`

### Challenge 1 - Password Pandemonium

Looks like we're tasked with updating a password for good ol `McSkidy`... somehow P@ssw0rd123 isn't secure which is utterly diabolical...

![[Pasted image 20251123180051.png]]

Anyways, ended up going for a random little string of words followed by a common number and symbol. Lots of passwords follow this structure but it's very hard to `bruteforce`. Exceptions to this is if it's apart of a large known dictionary of passwords. Like [rockyou.txt](https://github.com/josuamarcelc/common-password-list) and via a `dictionary bruteforce` attack they're able to either crack the hash, brute force a login page, etc.

![[Pasted image 20251123180245.png]]

### Challenge 2 - The Suspicious Chocolate.exe

According to the instructions provided it looks like we have a mystery USB on our desk and there's an executable on the drive. Personally, I would never plug in a USB you don't know or have a good idea of the contents on it unless it's brand new. If you were to do that I'd do it in a quarantined VM with malware analysis tools and monitoring programs installed [FlareVM](https://cloud.google.com/blog/topics/threat-intelligence/flare-vm-the-windows-malware) is personally one of my favorites. 

You can utilize normal Linux Distros for investigating and reverse engineering malware or bad scripts but personally most of the malware I play around is for Windows machines as [~87% of malware targets Windows operating systems](https://surfshark.com/research/chart/malware-cases-windows-macOS).
cybersecurity
![[Pasted image 20251123180638.png]]
![[Pasted image 20251123181302.png]]

After scanning the `chocolate.exe` file it's flagged as a `MalhareTrojan` from `Malhare Labs`. Generally it's very easy to change scripts, binaries, etc to personalize virus payloads for individual people/machines. Therefore, if you're getting a flagged virus on a checker like these. Generally there's a high chance you're dealing with an actual virus as most of these sites are kept up by publicly trusted vendors. 

For example, the real website **TryHackMe** is trying to encourage the user to leverage/understand is the actual [VirusTotal](https://www.virustotal.com/gui/home/upload).

Phenomenal website, you can investigate URLs, Files, IP Addresses. There's premium options, free options, etc. It's a very good tool I recommend anyone getting into cybersecurity leverages. 

### Challenge 3 - Welcome to the AttackBox!

This challenge it appears we're given the task of playing around on the THM attack box. Quick execution of help gives us the 5 available commands we get in this session.
- `ls` - list the content of your current directory unless clarified otherwise
- `cd` - change directory, you can move up or down the tree of directories via the directory naming conventions or built in functions. E.g, `cd ../../` will push you up or backwards two directories.
- `pwd` - present working directory. Will display your current directory e.g, `/home/kali`
- `cat` - stands for concatenate which is commonly used for displaying file contents in the terminal
- `clear` - simply clears out the current contents of your terminal.

![[Pasted image 20251123181929.png]]

### Challenge 4 - The CMD Conundrum

This challenge appears to be very similar to the last terminal interaction challenge. This time we have a `Windows Command Prompt`. Once again typing help gives you the available commands.

![[Pasted image 20251123184411.png]]

Funnily enough it shows the additional flags to the command `dir` which is the Linux equivalent of `ls`. Normally to see the additional features of a tool/function you need to call a help command on it. E.g, Linux: `ls -h : ls --help: man ls` Windows: `dir /? : dir /help`

### Challenge 5 - Linux Lore
![[Pasted image 20251123185947.png]]

This challenge is also similar to the previous one. You need to leverage the basic Linux terminal commands to traverse directories and find the hidden message key difference is the `-a` flag allows you to view the hidden directories and files. 
### Challenge 6 - The Leak in the List

![[Pasted image 20251123190104.png]]

You're just tasked with entering an email and seeing what or where it's been compromised in data breaches etc. The real world most commonly used tool for this is [HaveIBeenPwned](https://haveibeenpwned.com/).
### Challenge 7 - WiFi Woes in Wareville

![[Pasted image 20251123190228.png]]

This one you sign into the WiFi using default credentials and are tasked with securing the router login by changing the default password.

![[Pasted image 20251123190246.png]]

### Challenge 8 - The App Trap

It's reported that there's app permissions that are misconfigured giving an app access to send messages it's not supposed to be sending. Going through the apps and revoking any unnecessary perms yields you your flag.

![[Pasted image 20251123190340.png]]

![[Pasted image 20251123190405.png]]
### Challenge 9 - The Chatbot Confession

![[Pasted image 20251123190526.png]]

This one you just select any messages that a chat bot sent that leaks any potentially dangerous information. In the real world the best way to do this is not let LLMs or AI have access to the sensitive data, gap it from your network, and sanitize input and output from the LLM.
### Challenge 10 - The Bunny's Browser Trail

![[Pasted image 20251123190845.png]]
This one you're tasked with finding the unusual User Agent in the HTTP logs provided. Mozilla (Firefox), Edge, and Chrome are all common web browsers followed by common operating systems: MacOS, Windows, Linux. The odd one out here is `BunnyOS`.
## Skills Learned
Personally, I didn't really learn anything here but I decided to just bust it out for some XP on the THM website and in the oddity there was some good info for `Advent Cyber 2025`. Anyways, the point being I did the write up for the community mainly.