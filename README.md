# PerfCygWin
Add cpu perf meter to cygwin for windows platform

CPU performance API for Cgywin Installation on Windows

What is this software? This software is alternative monitor CPU performance without the windows resource monitor for Cygwin/gcc environments.  There is no really good way using Cygwin to get cpu usage in an API that will allow access cpu usage data.  Linux installations provide a virtual file system in which cpu stats are kept in /proc/loadavg.  To date there is no easy solution for Cygwin installations to provide an API for cpu performance.  This is one possible API that will give usage stats every second.

Does Cygwin provide an API to measure CPU performance?  A search of the Cygwin FAQ and mailing archives did find some relevance to the search CPU perf.  The results of this search revealed indicated the following:
From: "Chris January" <chris at atomice dot net>
To: jeff at joedog dot org, cygwin at cygwin dot com
Date: Wed, 27 Feb 2008 12:54:20 +0000
Subject: Re: /proc/loadavg
References: <39198.208.39.44.26.1204115358.squirrel@mail.tmorton.com>
On 27/02/2008, Jeff Fulmer wrote:
> I installed procps in the hopes of getting 1,5,15 minute load averages. I
>  have them, but the values to meet my expectations. No matter how much load
>  the system is under, the values are always 0.0 0.0 0.0 Are load averages
>  supported? Is there something I need to do in order to set them up?

>From the source:
//
//not really supported - Windows doesn't keep track of these values
     * Windows 95/98/me does have the KERNEL/CPUUsage performance counter
     * which is similar.
     //

To my knowledge there are no kernel counters on Windows
NT/2000/XP/Vista from which we could get these values.

Regards,
Chris

--
Unsubscribe info:      http://cygwin.com/ml/#unsubscribe-simple
Problem reports:       http://cygwin.com/problems.html
Documentation:         http://cygwin.com/docs.html
FAQ:                   http://cygwin.com/faq/

How does this software work?  Additional instructions will be provided in a readme file.

