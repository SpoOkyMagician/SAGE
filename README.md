This is a *very early* work in progress source code of SpoOky's Advanced Genie Editor... Sadly, it does not currently build but, I have made some decent progress.
I was trying to clean up the code wherever I could, fix issues, etc. The solution file is in the "build" directory.
(Release is currently the best build type. (64 Bit.) Debug is extremely broke.)
If anyone wants to help me out, I will gladly accept it. I am doing what I know I can. Some things however, I do not know how to do...

To summarize What I did: 1: I renamed all the files with a prefix from where they were from. Meaning:

A_*: Age Files.
G_*: Genie Files.
P_*: Pcrio Files.
U_*: Extra Files. (Not many but, I will look into them later on.)

2: I removed the precompile header stuff. (This was driving me nuts.) Besides, you could just add this to the top of main anyhow... Which I did.
I kept the old files for now just in case they are still needed but, I rather not go that route if need be.

3: I went though all the code, checked for anything I could possibly fix myself from suggestions. (I hope I didn't break anything. Although, I think they are fine...)

There is currently about 100 errors in the Release build. I could use your help with them.

Update: Well, I got it down to like 5 errors but, I am stuck on this wx smooth scrolling code...

Update: So, I *think* its even closer now but, now I am getting object errors now... This is frustrating... I had to temp remove 2 files because apparently, you can't include those files or msvc shows an error... Anyway, I had to remake the solution because I decided to go with a 32 bit application instead of 64 bit. I painstakingly build everything from source as 32 bit and used those libs. I believe I included everything.

Update: Minor fix and 19 errors now... Getting closer...

Update: Went though the include files and organized them in seperate files. (Trying to reduce the need for repeats and learning the code a bit.) I'll update the headers in A_Main in a bit...
