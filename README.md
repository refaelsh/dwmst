# dwmst
Status thingy for the DWM window manager.
# The story
I got fed up with bloated WMs (or worse, DEs like KDE/Gnome). I've been an avid Xfce fun for years, its very light, I highly recommended it. Until I stumbled on the [Tiling Window Managers](https://en.wikipedia.org/wiki/Tiling_window_manager) thingy. And I felt in heaven :-) Very fast, very light way, highly configurable, I can tinker with for hours on end :-) And the best one of all the tilling WMs is off course [DWM](https://suckless.org/) :-)
# Technical details
The following is mostly extracted from the [Great Arch Linux Wiki](https://wiki.archlinux.org/index.php/Dwm#Statusbar_configuration) article.
Note: The following requires the `xorg-xsetroot` package to be installed.
DWM reads the name of the root window and redirects it to the statusbar. The root window is the window within which all other windows are drawn and arranged by the window manager. Like any other window, the root window has a title/name, but it is usually undefined because the root window always runs in the background.

The information that you want dwm to show in the statusbar should be defined with `xsetroot -name ""` command in `~/.xinitrc` or `~/.xprofile` (if you are using a display manager). For example:

    xsetroot -name "Thanks for all the fish!"

Dynamically updated information should be put in a loop which is forked to background:

    # Statusbar loop
    while true; do
        xsetroot -name "$( date +"%F %R" )"
        sleep 1m    # Update time every minute
    done &

    # Autostart section
    pcmanfm & 
    exec dwm
    
In this case the date is shown in ISO 8601 format and PCManFM is launched at startup.

Note: It is not recommended to set the update interval equal to zero or remove the "sleep" line entirely since this will cause CPU usage to rise substantially (you can assess the effect with top and powertop) --> **And that is why I decided to implement it in C :-)**

# Purpose 
The purpose of this repo is for my own personal use. I will off course be very glad for any pull requests.
# State of project
Currently, it works. Its a mess, its a constant work in progress, but `master` always contains a code that works.
# Contributions
I am always glad when there is a pull request. I dare you :-)
# Technical details
Many parts of the code are taken from [here](https://github.com/drkhsh/slstatus).
