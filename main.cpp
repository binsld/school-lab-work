// Грузовой контейнер (имя груза, масса, объём, категория (пустой, штучный, насыпной, жидкость, газ, опасный)))
// можно перекладывать груз из одного контейнера в другой, но только того же типа 
// но насыпной, жидкость, газ можно ссыпать вместе при одинаковом наименовании
// объединять опасные грузы нельзя 
// что угодно можно пересыпать в пустой контейнер

// V конструктор с наименованием, массой, категорией, объёмом
// V экземпляр класса только с массой и объёмом (уже готовый груз для которого нужно указать параметры)
// V получение плотности груза
// V перегрузка массы из одного контейнера в другой
// V добавить массу в контейнера.

#include <iostream>
#include <vector>
using namespace std;

enum types {empty, piece, bulk, liquid, gas, dangerous};

class container
{
    protected:
    public:
        string name;
        float mass, volume;
        types category;
        container(string n, float m, float v, types c)
        {
            if (m <= 0 || v <= 0) throw "Is your cargo paranormal ?";
            name = n;
            mass = m;
            volume = v;
            category = c;
        }
        container()
        {
            name = "Пусто";
            mass = 0;
            volume = 0;
            category = types::empty;
        }
        void move_cargo(container &c, float m = -1)
        {
            if (m == -1) m = mass;
            float v = volume/mass*m;
            if (mass < m) throw "There is not enough cargo in container";
            if (c.category == dangerous || category == dangerous) throw "Cargo is too dangerous to do this";
            if (c.category == category) {
                c.mass += m;
                c.volume += v;
                volume -= v;
                mass -= m;
                return;
            }
            else if ((c.category == types::bulk || c.category == types::liquid || c.category == types::gas) && (category == types::bulk || category == types::liquid || category == types::gas)) {
                if (name != c.name) throw "Cargo cannot be combined";
                c.mass += m;
                mass -= m;
                return;
            }
            else if (c.category == types::empty)
            {
                c.mass = m;
                c.volume = volume;
                c.name = name;
                c.category = category;
                mass -= m;
                return;
            }
            throw "You can't do this";
        }
        float get_density()
        {
            if (volume <= 0 || mass <= 0) throw "Is your cargo paranormal ?"; 
            return mass / volume;
        }

        string get_name() {return name;}
        float get_mass() {return mass;}
        float get_volume() {return volume;}
        types get_category() {return category;}

        void add_mass(float m)
        {
            volume = volume / mass * (mass + m);
            mass += m;
        }

};

// Методы сложного класса
// V создание экземпляров класса с инициализацией заданным количеством контейнеров из массива контейнеров;
// V создание экземпляров класса с инициализацией одним контейнером;
// V добавление нового контейнера;
// V получение контейнера по его номеру (возврат по ссылке);
// V удаление контейнера по его номеру;
// V подсчёт суммарной массы всех контейнеров;
// V вычисление центра массы поезда, измеряемой в контейнерах (считаем, что все контейнеры имеют одинаковую длину);

class train
{
    private:
        vector<container> cargo;
        unsigned short max_payload;                               // сумма масс грузов не может привышать это значение
        unsigned short max_container_volume;                      // объём каждого контейнера не может привышать это значение
        // float cache_mass;

    public:
        train(initializer_list<container> containers)
        {
            for (container i: containers)
            {
                cargo.push_back(i);
            }
        }
        
        train(container cont)
        {
            cargo.push_back(cont);
        }
        
        float sum_mass()
        {
            float mass;
            for (unsigned short i = 0; i < cargo.size(); i++)
            {
                mass += cargo[i].get_mass();
            }
            return mass;
        }

        unsigned short get_center_of_mass()
        {
            float massc = 0, mass = sum_mass();
            for (unsigned short i = 0; i < cargo.size(); i++)
            {
                massc += cargo[i].get_mass();
                if (massc > mass/2)
                {
                    if (i == 0) return 0;
                    return i - 1;
                }
            }
        }

        void add_container(container cont)
        {
            if (cont.get_mass() + sum_mass() > max_payload) throw "Too hard";
            if (cont.get_volume() > max_container_volume) throw "Too big";
            cargo.push_back(cont);
        }

        container* get_container(unsigned short num)
        {
            if (num >= cargo.size()) throw "Container not found";
            return &cargo[num];
        }

        void delete_container(unsigned short num)
        {
            if (num >= cargo.size()) throw "Container not found";
            cargo.erase(cargo.cbegin() + num); 
        }

};

int main()
{
    container a("Water", 1.0f, 2.2f, types::liquid);
    container b("Water", 2.0f, 4.4f, types::liquid);
    b.move_cargo(a, 0.5);
    container uranium("Uranium", 5.0f, 3.0f, types::dangerous);
    cout << "Name: " << a.get_name() << endl << "Mass: " << a.get_mass() << endl << "Volume: " << a.get_volume() << endl << "Category: " << a.get_category() << endl;
    train t{a,b,uranium};
    cout << t.get_container(2)->name << ' ' << t.get_container(2)->mass << "kg" << endl;
}