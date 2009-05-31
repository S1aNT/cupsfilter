#! /bin/bash 
# Коды возвратов
E_BADARGS=80            # не определены все аргументы
E_PROFILE_FOUND=81      # профиль с таким именем существует
E_PROFILE_NOTFOUND=87   # профиль с таким именем существует
E_PROFILE_CREATED=82    # профиль успешно создан
E_PROFILE_DELETED=84    # профиль успешно удалён
E_PROFILE_NODELETED=86  # профиль не существует
E_PROFILE_READED=85     # профиль успешно прочитан
E_PROFILE_MODIFIED=83   # профиль успешно модифицирован
E_VM_READ=68            # IBMUSER в состоянии VM READ

# Переменные
nameclass='$PRT'      # имя используемого класса
namevmobi=PRT         # имя ВМ с функциями SPECIAL - для выполнения команд СУРД, должна быть зарегистрирована и находиться в состоянии RUNNING
seclabel=""         # метка защиты профиля
nameprinter=""      # имя защищаемого устройства печати

# Передаваемые аргументы
# $1 - имя принтера(не более 30 символов), $2 - мандат пользователя 
# $3 - выполняемое программой действие: 
# * 1- добавить профиль,
# * 2 - изменить профиль,
# * 3 - прочитать данные о профиле в файл,
# * 4 - удалить профиль)

# НАЧИНАЕМ 
# с анализа входных аргументов
if [ $# -eq 0 ]  # не указаны аргументы.
then
  echo "Количество аргументо: "$#
  echo "Порядок использования команды: `basename $0` printer_name seclabel [action]";
	echo " Передаваемые аргументы:";
	echo "$1 - имя принтера(не более 30 символов),"; 
	echo "$2 - мандат пользователя (не более 6 символов),"; 
	echo "$3 - выполняемое программой действие(цифра от 1 до 4):"; 
	echo " - 1 - добавить профиль,";
	echo " - 2 - изменить профиль,";
	echo " - 3 - прочитать данные о профиле в файл,";
	echo " - 4 - удалить профиль.";
  exit $E_BADARGS
fi

if [ -z "$1" ]  # Имя принтера не указаны.
then
  echo "Порядок использования команды: `basename $0` printer_name seclabel [action]"
  exit $E_BADARGS
fi
nameprinter=$1

if [ -z "$2" ]  # мандат не указан.
then
  echo "Порядок использования команды: `basename $0` printer_name seclabel [action]"
  exit $E_BADARGS
fi
seclabel=$2

if [ -z "$3" ]  # действие не указано.
then
  action="1"      # если не указано, то по умолчанию - добавить профиль
else
  action=$3       # в противном случае берём из командной строки
fi

echo "Начало работы программы - `date`"  
# echo "Создаём в СУРДе профиль защиты для принтера \"$nameprinter\" с меткой защитой \"$seclabel\" "
#  echo $HOSTNAME.$nameprinter
  nameprofile=`echo $HOSTNAME.$nameprinter | tr '[a-z]' '[A-Z]'` 
#  echo $nameprofile

# Проверям доступ к 191 минидиску с результатами выполнения команд
 if [ `hcp q v 1 | awk '{print $1}'` = "HCPQVD040E" ] ; then
 echo "Подключим 191 минидиск от ВМ ${namevmobi}";
 hcp link ${namevmobi} 0191 0001 rr rprt;
 echo "Код возврата из команды link- "$?
 fi
 hcp q n | grep "${namevmobi}"
if [ `echo $?` != "0" ] ; then
echo "ВМ ${namevmobi} незарегистрирована.Выполняем регистрацию."
hcp XAUTOLOG ${namevmobi}
fi

# Проверяем доступ к консоли ВМ IBMUSER
echo "Проверка доступа к консоли ВМ ${namevmobi}"
hcp send ${namevmobi} "rac search class($nameclass) filter($nameprofile)"
if [ `echo $?` = "68" ] ; then
echo "Невозможно выполнить команду `basename $0`, так как консоль ${namevmobi} в состоянии VM READ.Код возврата $? "
echo "Завершение работы программы - `date`"
exit $E_VM_READ
fi

case $action in
1)
echo "Ищем в СУРД профиль с таким же именем"
hcp send ${namevmobi} "rac search class($nameclass) filter($nameprofile)"
 chccwdev -d 0001
 chccwdev -e 0001                   # переводим в online устройство с адресом 1
# смотрим содержимое файла racf.data
cmsfscat -d /dev/$(lsdasd 1 | awk '{print $7}') -a racf.data | grep "$nameprofile"   
if [ `echo $?` = "0" ] ; then
echo "Профиль с именем ${nameprofile} уже существует.Код возврата ${E_PROFILE_FOUND} "
echo "Завершение работы программы - `date`"
exit $E_PROFILE_FOUND
fi
echo "Поик завершён - профиль не найден" 
echo "Создаём профиль"
hcp send ${namevmobi} "rac rdefine $nameclass $HOSTNAME.$nameprinter seclabel($seclabel) uacc(read)"
chccwdev -d 0001
chccwdev -e 0001                   # переводим в online устройство с адресом 1
# смотрим содержимое файла racf.data
 cmsfscat -d /dev/$(lsdasd 1 | awk '{print $7}') -a racf.data  
# если всё удачно, то сообщение на консоль - file not found с кодом воврата 255
if [ `echo $?` = "255" ] ; then
echo "Профиль с именем ${nameprofile} успешно создан.Код возврата ${E_PROFILE_CREATED} "
else
echo "Профиль с именем ${nameprofile} не создан. Код возврата -" $?
echo "Завершение работы программы - `date`"
exit $E_PROFILE_CREATED
fi
;;

2)
echo "Модифицируем профиль"
hcp send ${namevmobi} "rac ralter $nameclass $HOSTNAME.$nameprinter seclabel($seclabel)"
chccwdev -d 0001
chccwdev -e 0001                   # переводим в online устройство с адресом 1
# смотрим содержимое файла racf.data
 cmsfscat -d /dev/$(lsdasd 1 | awk '{print $7}') -a racf.data  
# если всё удачно, то сообщение на консоль - file not found с кодом воврата 255
if [ `echo $?` = "255" ] ; then
echo "Профиль с именем ${nameprofile} успешно модифицирован.Код возврата ${E_PROFILE_MODIFIED} "
echo "Завершение работы программы - `date`"
exit $E_PROFILE_MODIFIED
fi
;;

3) 
echo "Ищем в СУРД профиль с именем $nameprofile"
hcp send ${namevmobi} "rac search class($nameclass) filter($nameprofile)"
 chccwdev -d 0001
 chccwdev -e 0001                   # переводим в online устройство с адресом 1
# смотрим содержимое файла racf.data
cmsfscat -d /dev/$(lsdasd 1 | awk '{print $7}') -a racf.data | grep "$nameprofile"   
if [ `echo $?` != "0" ] ; then
echo "Профиль с именем ${nameprofile} не существует."
exit $E_PROFILE_NOTFOUND
fi
echo "Поик завершён - профиль найден" 
echo "Читаем метку защиты профиля"
hcp send ${namevmobi} "rac rlist $nameclass $nameprofile all"
chccwdev -d 0001
chccwdev -e 0001                   # переводим в online устройство с адресом 1
# смотрим содержимое файла racf.data
 cmsfscat -d /dev/$(lsdasd 1 | awk '{print $7}') -a racf.data | 
# ограничиваем диапазон строк файла ключивыми словами
awk '$1 == "SECLABEL", $1 == "AUDITING"' |
# оставляем из выбранного диапазона первые 3 строки
head -3 |
# и отображаем только последнюю из них
sed '$!d' >/var/tmp/seclabel

#/var/tmp/${nameprofile}.txt
#ls -la /var/tmp/${nameprofile}.txt  
# если всё удачно, то сообщение на консоль - file not found с кодом воврата 255
if [ `echo $?` = "0" ] ; then
echo "Метка профиля с именем ${nameprofile} успешно прочитана в файл /var/tmp/seclabel. Код возврата ${E_PROFILE_READED} "
#echo "Завершение работы программы - `date`"
exit $E_PROFILE_READED
fi
;;

4) 
echo "Удаляем профиль"
hcp send ${namevmobi} "rac rdel $nameclass $HOSTNAME.$nameprinter"
chccwdev -d 0001
chccwdev -e 0001                   # переводим в online устройство с адресом 1
# смотрим содержимое файла racf.data
 cmsfscat -d /dev/$(lsdasd 1 | awk '{print $7}') -a racf.data  
# если всё удачно, то сообщение на консоль - file not found с кодом воврата 255
if [ `echo $?` = "255" ] ; then
echo "Профиль с именем ${nameprofile} успешно удалён.Код возврата ${E_PROFILE_DELETED} "
echo "Завершение работы программы - `date`"
exit $E_PROFILE_DELETED
else
echo "Удаление невозможно - профиль с именем ${nameprofile} не существует"
echo "Завершение работы программы - `date`"
exit $E_PROFILE_NODELETED
fi
;;
esac
