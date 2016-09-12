#
# Regular cron jobs for the steps package
#
0 4	* * *	root	[ -x /usr/bin/steps_maintenance ] && /usr/bin/steps_maintenance
