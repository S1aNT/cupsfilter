#!/bin/sh

FILTER_FILE=/opt/filter/spts_filter.pl
CUPS_FILTER_DIR=/usr/lib/cups/filter

# Create the audit's directory
if [ ! -d /var/log/cups/audit ] ; then
   mkdir /var/log/cups/audit
   chown root:lp /var/log/cups/audit
   chmod 775 /var/log/cups/audit
fi

# Create the firstpages directory
if [ ! -d /var/log/cups/firstpages ] ; then
   mkdir /var/log/cups/firstpages
   chown root:lp /var/log/cups/firstpages
   chmod 775 /var/log/cups/firstpages
fi

if [ -f $FILTER_FILE ]; then
    ln -s $FILTER_FILE $CUPS_FILTER_DIR/spts_filter
fi

exit 0;
