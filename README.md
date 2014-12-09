HTML Parser
===========

A simple and small HTML parser written in C++ 11. The current API for interacting with the parsed HTML tree structure is rudimentary.


Why?
----

I wrote this because I wanted to analyze some web pages, but I wanted all my code to be as small and portable as possible (but mostly my own curiosity).

However, it all began one morning while taking a shower when I was struck by the thought *"wait, HTML has a pretty simple structure. It's like XML but with fewer weird features. I could totally build a way of interacting with HTML!"*

As it turns out, yes, I can build that. And I have. The only downside is...

Uhhh, but this stinks?
----------------------

Well, yeah! I've never written a parser before, I don't know how any of this stuff is *supposed* to work! I actually had a working version that was **terrible** before I even looked at the Wikipedia page about parsers. I've since completely re-written it to be less spaghetti-like, but that should give you a good idea of my state of mind while building this.

I HIGHLY advise you never, EVER use this on anything that matters. However, if you'd like to suggest changes I'd love to hear them! In fact, PLEASE TELL ME MORE ABOUT WHAT IS BAD ABOUT THIS! Why does it suck? What basic conventions of C++ have I overlooked in building this? If you where building this, how you build it?

