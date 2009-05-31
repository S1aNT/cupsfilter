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

namevmobi='PRT'
# НАЧИНАЕМ 
echo "Начало работы программы - `date`"  

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


echo "Ищем в СУРД все метки из класса seclabel"
hcp send ${namevmobi} "rac search class(seclabel)"
chccwdev -d 0001
chccwdev -e 0001                   # переводим в online устройство с адресом 1
# смотрим содержимое файла racf.data
 cmsfscat -d /dev/$(lsdasd 1 | awk '{print $7}') -a racf.data  > /var/tmp/seclabel_list
ls -la /var/tmp/seclabel_list  
# если всё удачно, то сообщение на консоль - file not found с кодом воврата 255
if [ `echo $?` = "0" ] ; then
echo "Метки успешно прочитаны в файл /var/tmp/seclabel_list. Код возврата ${E_PROFILE_READED} "
echo "Завершение работы программы - `date`"
exit $E_PROFILE_READED
fi

