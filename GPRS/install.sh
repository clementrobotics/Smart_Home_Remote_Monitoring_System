#!/bin/sh
clear
echo " "
#declare name="ds"
echo "�������û���:"
read user_name
echo "��������"
read passwd
case $user_name in
	"ds")
		echo "��ӭ,$user_name."
			case $passwd in
			    "123456")
				echo "******************"
				insmod ./drivers/demo.o
				insmod ./drivers/dc-motor.o
				insmod ./drivers/s3c2410-adc.o
				./gprs_4.0/gprs
				;;
			     *)
				echo "*******************"
				echo "�������"
				;;
			esac
			;;
	*)
		 echo "�����û�������"
		  ;;
esac



