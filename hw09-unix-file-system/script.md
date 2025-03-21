### Unix console

- Узнайте полный путь текущего каталога.
  ```shell
  pwd
  ```
- Выведите на экран ПОЛНОЕ содержимое текущего каталога.
  ```shell
  ls -la
  ```
- Создайте каталог otusHW.
  ```shell
  mkdir otusHW
  ```
- Перейдите в каталог otusHW.
  ```shell
  cd otusHW
  ```
- Выведите в консоль информацию о файловой системе (точки монтирования, занимаемый размер, свободное место).
  ```shell
  df -h
  ```
- Информацию из пункта 5 выведите в файл fsInfo.
  ```shell
  df -h > fsInfo
  ```
- Выведите на экран содержимое файла fsInfo.
  ```shell
  cat fsInfo
  ```
- Выведите на экран строчки файла fsInfo, в которых есть цифра 1, если таких нет, то цифра 2.
  ```shell
  grep '1' fsInfo || grep '2' fsInfo
  ```
  или
  ```shell
  cat fsInfo | grep 1 || cat fsInfo | grep 2
  ```
- Сделайте копию файла fsInfo - fsInfoСopy.
  ```shell
  cp fsInfo fsInfoCopy
  ```
- Сделайте копию файла fsInfo - fsInfoСopy2.
  ```shell
  cp fsInfo fsInfoCopy2
  ```
- Переименуйте fsInfoСopy2 в fsInfoСopy3.
  ```shell
  mv fsInfoCopy2 fsInfoCopy3
  ```
- Удалите fsInfoСopy3.
  ```shell
  rm fsInfoCopy3
  ```
- Выведите на экран первые строки fsInfoСopy.
  ```shell
  head fsInfoCopy
  ```
- Выведите на экран последние строки fsInfoСopy.
  ```shell
  tail fsInfoCopy
  ```
- Создайте файл runIt.
  ```shell
  touch runIt
  ```
- В vi откройте файл runIt.
  ```shell
  vi runIt
  ```
- В файл runIt добавьте команду копирования fsInfo в fsInfoFinal.
  > Команды в vi:  
  > `i` - перейти в режим редактирования  
  > > cp fsInfo fsInfoFinal 
  > 
  > `Esc` - перейти в командный режим  
  > `:wq` - сохранить и выйти
- Сделайте runIt исполняемым.
  ```shell
  chmod a+x runIt
  ```
  или
  ```shell
  chmod 755 runIt
  ```
- Выполните runIt и убедитесь, что он отработал как надо.
  ```shell
  ./runIt
  ```
  > `ls -l`  
  > total 32  
  > -rw-r--r-- 1 ludalova staff 1235 Mar 21 15:49 fsInfo  
  > -rw-r--r-- 1 ludalova staff 1235 Mar 21 16:01 fsInfoCopy  
  > -rw-r--r-- 1 ludalova staff 1235 Mar 21 16:59 fsInfoFinal  
  > -rwxr-xr-x 1 ludalova staff 22 Mar 21 16:41 runIt
