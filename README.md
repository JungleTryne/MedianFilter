# Median Filter

Данила Мишин, Б05-922

### Сборка

```bash
mkdir build
cd build
cmake ..
make
```

### Запуск
```bash
./MedianFilter --radius 25 --filepath ../resources/cat.bmp --algorithm constant
```
Для вышеописанного примера мы запускаем медианный фильтр радиуса 25
на картинке кота по относительному пути `../resources/cat.bmp`,
используя алгоритм с асимптотикой `O(1)`. Доступны следующие
алгоритмы

- `simple` - наивная реализация медианного фильтра
- `huang` - алгоритм Huang et al.
- `constant` - алгоритм за `O(1)`
- `opencv` - реализация opencv (для сравнения)

Примеры работы для радиусов 9 и 25 можено посмотреть в
папке resources. Там же можно найти фотку кота.

Результат работы программы совпадает для всех 
вышеописанных алгоритмов.

### Бенчмарк

Также для сравнения скорости работы был написан простой benchmark
на Python, он расположен в папке benchmark. Пример
его запуска:

```bash
python3 main.py --program-path ../cmake-build-release-system/MedianFilter --image-path ../resources/cat.bmp
```

В `requirements.txt` указаны все требуемые модули для работы
бенчмарка. В той же папке доступны графики зависимости
скорости работы алгоритмов (мсек/мегапиксель)
от радиуса окна. Для алгоритма `simple` бенчмарк не
запускался для R > 10 в силу неоптимальности алгоритма.

### Фактическая асимптотика

Пусть дано изображение `h x w`, фильтр радиуса `R`. 
Считаем изображение трехканальным. Тогда 

- `simple`:
`O(h * w * R * logR)`

- `huang`:
`O(h * w * R)`

- `constant`
`O(h * w)`

Причем две последние оценки амортизированы, так как
инициализация окна может занять `O(R^2)`, но дальнейший
проход будет осуществлен за `O(R)` и `O(1)` (соответсвенно
для алгоритмов)
для каждого шага прохода.

### Оптимальная композиция алгоритмов

На графиках видно, что начиная с `R > 100` алгоритм
`constant` работает лучше, чем `huang`. Оно логично, ведь
для мелких R в `huang` приходится проходиться по меньшему
количеству значений (по `O(R)` значениям, вместо `256`).

Понятно, что приведенная реализация далека от оптимальной.
При оптимизации алгоритмов эта граница может поменяться.
Но в данной ситуации она равна `R = 100`. До нее надо
использовать `huang`, после нее - `constant`