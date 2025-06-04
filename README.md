# Процесс Сборки

1. Перейти в корень диска C.
2. git clone https://github.com/microsoft/vcpkg.git
3. cd vcpkg
4. .\bootstrap-vcpkg.sh
5. .\vcpkg.exe integrate install
6. Скопировать текст после CMake projects should use: ...
7. .\vcpkg install gtest
8. скачать архи библиотеки https://www.sfml-dev.org/files/SFML-3.0.0-windows-vc17-64-bit.zip и распаковать в корень диска С.
9. git clone https://github.com/Sqaz33/fun_of_the_gods.git
10. cd fun_of_the_gods
11. mkdir build
12. cmake .. \<скопированный текст, который -DCMAKE...\>
13. cmake --build .
14. можно запускать бинарники.
