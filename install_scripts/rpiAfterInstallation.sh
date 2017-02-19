echo "tmpfs	/tmp		tmpfs	defaults	0	0" >> /etc/fstab
echo "tmpfs	/var/tmp	tmpfs	defaults	0	0" >> /etc/fstab

# Disable logging in /etc/rsyslog.conf and /etc/rsyslog.d/*
#auth,authpriv.*        /var/log/auth.log
#*.*;auth,authpriv.none -/var/log/syslog
#cron.*                 /var/log/cron.log
#daemon.*               -/var/log/daemon.log
#kern.*                 -/var/log/kern.log
#lpr.*                  -/var/log/lpr.log
#mail.*                 -/var/log/mail.log
#user.*                 -/var/log/user.log

FILE=/etc/rsyslog.conf

sed s/^auth,/#auth./ $FILE
sed s/^\*\.\*/#\*\.\*/ $FILE
sed s/^cron/#cron/ $FILE
sed s/^daemon/#daemon/ $FILE
sed s/^kern/#kern/ $FILE
sed s/^lpr/#lpr/ $FILE
sed s/^mail/#mail/ $FILE
sed s/^user/#user/ $FILE

#Avoid more logging
#ln -s /dev/null alternatives.log

echo "" >> /etc/network/interfaces
echo "wireless-power off" >> /etc/network/interfaces
#turn off journaling
#tune2fs -O ^has_journal /dev/sdbX

#$ sudo kpartx -av image_you_want_to_convert.img 
#add map loop0p1 (252:5): 0 117187 linear /dev/loop0 1
#add map loop0p2 (252:6): 0 3493888 linear /dev/loop0 118784
#sudo mount /dev/mapper/loop0p2 /mnt
#sudo sed -i 's/^\/dev\/mmcblk/#\0/g' /mnt/etc/fstab
#sudo mksquashfs /mnt converted_image_for_berryboot.img -comp lzo -e lib/modules
#sudo umount /mnt
#sudo kpartx -d image_you_want_to_convert.img 

apt-get update
apt-get upgrade
apt-get dist-upgrade
