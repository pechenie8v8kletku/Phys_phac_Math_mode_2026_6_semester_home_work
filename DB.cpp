#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <functional>
#include <map>
#include <numeric>

struct Student {

    int id;
    std::string name;
    int birth_year;
    int group;
};

template<typename Key>
class GroupQuery {

private:

    std::map<
        Key,
        std::vector<const Student*>
    > groups;

public:

    GroupQuery(
        std::map<
        Key,
        std::vector<const Student*>
        > grouped_data
    )
        :
        groups(std::move(grouped_data))
    {
    }

    void print_groups() const {

        for (
            const auto& pair
            : groups
            ) {

            const auto& key =
                pair.first;

            const auto& vec =
                pair.second;

            std::cout
                << "\nGROUP: "
                << key
                << "\n";

            std::for_each(
                vec.begin(),
                vec.end(),
                [](const Student* s) {

                    std::cout
                        << "ID: " << s->id
                        << ", Name: " << s->name
                        << ", Birth year: " << s->birth_year
                        << ", Group: " << s->group
                        << '\n';
                }
            );
        }
    }
    void count() const {

        std::cout
            << "\n===== COUNT =====\n";

        for (
            const auto& pair
            : groups
            ) {

            std::cout
                << pair.first
                << " -> "
                << pair.second.size()
                << '\n';
        }
    }
};

class Query {

private:

    std::vector<const Student*> data;

public:

    Query() = default;

    Query(
        std::vector<const Student*> students
    )
        :
        data(std::move(students))
    {
    }

    Query& where(
        std::function<bool(const Student&)> predicate
        ) {

        data.erase(
            std::remove_if(
                data.begin(),
                data.end(),
                [&](const Student* s) {

                    return
                        !predicate(*s);
                }
            ),
            data.end()
        );

        return *this;
    }

    Query& order_by_name() {

        std::sort(
            data.begin(),
            data.end(),
            [](const Student* a, const Student* b) {

                return
                    a->name
                    < b->name;
            }
        );

        return *this;
    }

    Query& order_by_birth_year() {

        std::sort(
            data.begin(),
            data.end(),
            [](const Student* a, const Student* b) {

                return
                    a->birth_year
                    < b->birth_year;
            }
        );

        return *this;
    }

    Query& limit(
        size_t n
    ) {

        if (
            data.size() > n
            ) {

            data.resize(n);
        }

        return *this;
    }

    template<typename Func>
    auto group_by(
        Func key_selector
    ) const {

        using Key =decltype(key_selector(std::declval<Student>()));

        std::map<Key,std::vector<const Student*>> grouped;

        std::for_each(
            data.begin(),
            data.end(),
            [&](const Student* s) {grouped[key_selector(*s)].push_back(s);
            }
        );

        return GroupQuery<Key>(grouped);
    }

    void print() const {

        std::cout
            << "\n===== RESULT =====\n";

        std::for_each(
            data.begin(),
            data.end(),
            [](const Student* s) {

                std::cout
                    << "ID: " << s->id
                    << ", Name: " << s->name
                    << ", Birth year: " << s->birth_year
                    << ", Group: " << s->group
                    << '\n';
            }
        );
    }
};

class StudentDB {

private:

    std::unordered_map<
        int,
        Student
    > students;

public:

    void add_student(
        const Student& s
    ) {

        if (
            students.find(s.id)
            != students.end()
            ) {

            std::cout
                << "Student already exists\n";

            return;
        }

        students[s.id] = s;
    }

    Query select() const {

        std::vector<const Student*> result;

        for (
            const auto& pair
            : students
            ) {

            result.push_back(
                &pair.second
            );
        }

        return Query(result);
    }

    void save_to_file(
        const std::string& filename
    ) const {

        std::ofstream out(filename);

        std::for_each(
            students.begin(),
            students.end(),
            [&](const auto& pair) {

                const Student& s =
                    pair.second;

                out
                    << s.id << ';'
                    << s.name << ';'
                    << s.birth_year << ';'
                    << s.group << '\n';
            }
        );
    }

    void load_from_file(
        const std::string& filename
    ) {

        students.clear();

        std::ifstream in(filename);

        std::string line;

        while (
            std::getline(in, line)
            ) {

            std::stringstream ss(line);

            Student s;

            std::string temp;

            std::getline(ss, temp, ';');
            s.id =
                std::stoi(temp);

            std::getline(
                ss,
                s.name,
                ';'
            );

            std::getline(ss, temp, ';');
            s.birth_year =
                std::stoi(temp);

            std::getline(ss, temp, ';');
            s.group =
                std::stoi(temp);

            students[s.id] = s;
        }
    }
};

int main() {

    StudentDB db;

    db.load_from_file(
        "students.txt"
    );

    db.add_student({
        1,
        "Alex",
        2004,
        241
        });

    db.add_student({
        2,
        "Kirill",
        2003,
        241
        });

    db.add_student({
        3,
        "Nikita",
        2005,
        242
        });

    db.add_student({
        4,
        "Ivan",
        2002,
        242
        });

    db.add_student({
        5,
        "Anna",
        2004,
        241
        });

    db.add_student({
        6,
        "Maria",
        2001,
        243
        });

    db.select().where([](const Student& s) {return  s.group == 241; }).order_by_name().limit(2).print();

    db.select().where([](const Student& s) {return s.birth_year < 2004; }).group_by([](const Student& s) {return s.group;}).print_groups();

    db.select().group_by([](const Student& s) {return s.group;}).count();

    db.save_to_file(
        "students.txt"
    );
}