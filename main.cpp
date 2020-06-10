#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <iterator>
#include <cassert>

using namespace std;

int sampleID = 1;
int elementID = 1;

class Sample
{
public:
    int amount;
    int lengthBase;
    int lengthAltered;
    vector<int> *partitions;

    explicit Sample(int _length)
    {
//        ID = sampleID;
        amount = 1;
        lengthBase = _length;
        lengthAltered = lengthBase;
        partitions = new vector<int>();
    }

    void divide(int _length)
    {
        assert(_length <= lengthAltered);
        lengthAltered -= _length;
        partitions->push_back(_length);
    }

    void add(int how_many)
    {
        amount += how_many;
    }

    bool operator==(const Sample &other) const
    {
        if (other.partitions->size() != this->partitions->size())
            return false;
        for (auto it1 = this->partitions->begin(), it2 = other.partitions->begin();
             it1 != this->partitions->end(); it1++, it2++)
        {
            if (*it1 != *it2)
                return false;
        }
        return true;
    }

    void print() const
    {
        cout << amount << ": ";
        for (int part : *partitions)
            cout << part << " ";
        cout << endl;
    }
};

class Element
{
public:
    int ID;
    int lengthBase;
    int lengthAltered;
    vector<int> sampleIDs;

    explicit Element(int _length)
    {
        ID = elementID;
        lengthBase = _length;
        lengthAltered = _length;
    }

    void assign(int _id, int _length)
    {
        assert(_length <= lengthAltered);
        lengthAltered -= _length;
        sampleIDs.push_back(_id);
    }
};

//inserts given elem into vec (in appropriate place
void autoinsert(vector<Element> *vec, Element elem)
{
    //This is not Bin Search, needs optimising!
    //Ultra unoptimised!!! (what did you want with emplace)
    auto it = vec->begin();
    for (it; it != vec->end(); ++it)
    {
        if (elem.lengthAltered < it->lengthAltered)
        {
            vec->emplace(it, elem);
            return;
        }
    }
    vec->emplace_back(elem);
}

//returns index of element with max lengthAltered that is less or equal to length
//or -1 if it does not exist
int binSearchMaxLessOrEqual(vector<Element> vec, int length)
{
    //This is not Bin Search, needs optimising!
    int j = -1;
    for (auto it = vec.begin(); it != vec.end(); ++it, ++j)
    {
        if (it->lengthAltered > length)
        {
//            cout << it->lengthAltered << " is greater then " << length << ", index " << j << endl;
            break;
        }
    }
    return j;
}

//searches if there's exact same sample in vector of Sample-s and stacks with it
//or adds new sample in vector otherwise
void sampleCrunch(vector<Sample> *vec, Sample smp)
{
    for (auto it = vec->rbegin(); it != vec->rend(); ++it)
    {
//        it->print();
//        smp.print();
        if (smp == *it)
        {
//            cout << "YAY!\n";
            it->add(1);
            return;
        }
//        cout << "nay...\n";
    }
    vec->emplace_back(smp);
}

int main()
{
    fstream in;
    ofstream out;
    setlocale(LC_ALL, "Russian");
    in.open("input.txt");
    out.open("output.txt");
    cout << "----------------------\n";
    cout << "PlankValue Калькулятор\n\n";
    if (in.is_open() and in)
    {
        if (out.is_open() and out and out.good())
        {
            int elements_num;
            int wdTemplate;
            int limit;
            map<int, int> elements_map;
            int in_var1, in_var2;

            //template
            in >> wdTemplate;

            //limit
            in >> limit;

            //elements
            in >> elements_num;
            for (int i = 0; i < elements_num; ++i)
            {
                in >> in_var1 >> in_var2;
                elements_map[in_var1] += in_var2;
            }

            //sum calculation
            int elem_sum = 0;
            int trash = 0;
            for (auto element: elements_map)
            { elem_sum += element.first * element.second; }
            //formatting
            vector<Element> elements;
            for (auto element: elements_map)
            {
                for (int i = 0; i < element.second; ++i)
                {
                    Element one_more(element.first);
                    elements.push_back(one_more);
                }
            }

            //algorithm
            vector<Sample> samples;
//            vector<Element> answer;

            while (binSearchMaxLessOrEqual(elements, wdTemplate) < elements.size() - 1)
            {
                Element elem = elements.back();
                elements.pop_back();

                //Substracting from biggest
                while (elem.lengthAltered > wdTemplate)
                {
                    Sample sample1(wdTemplate);
                    sampleID++;
                    elem.assign(sample1.amount, sample1.lengthBase);
                    sample1.divide(sample1.lengthBase);
//                    samples.emplace_back(sample1);
                    sampleCrunch(&samples, sample1);
                }

                if (elem.lengthAltered > 0)
                    autoinsert(&elements, elem);
                else
                {
//                    answer.emplace_back(elem);
                    elementID++;
                }
            }

            while (!elements.empty())
            {
                Sample sample1(wdTemplate);
                sampleID++;
                while (sample1.lengthAltered >= limit)
                {
                    if (elements.empty())
                        break;
                    int index = binSearchMaxLessOrEqual(elements, sample1.lengthAltered);
                    if (index < 0)
                        break;
                    Element elem1 = elements[index];

                    elementID++;
                    elements.erase(elements.begin() + index);
                    int len = elem1.lengthAltered;
//                    cout << len << " " << sample1.lengthAltered << endl;
                    elem1.assign(sample1.amount, len);
                    sample1.divide(len);
                    elementID++;
//                    answer.emplace_back(elem1);
                }
                if (sample1.lengthAltered > 0)
                    trash += sample1.lengthAltered;
                sampleCrunch(&samples, sample1);
            }

            //output
            cout << "Заготовки:\n";
            for (const auto &sample : samples)
            {
                cout << sample.amount << ": ";
                for (int part : *sample.partitions)
                    cout << part << " ";
                cout << endl;
            }
            for (const auto &sample : samples)
            {
                out << sample.amount << " ";
                for (int part : *sample.partitions)
                    out << part << " ";
                out << endl;
            }
//            cout << "Ответ:\n";
//            for (const auto &ans : answer)
//            {
//                cout << ans.ID << ": ";
//                for (int sid : ans.sampleIDs)
//                    cout << sid << " ";
//                cout << endl;
//            }

            cout << "\nСрез: " << trash << "/" << elem_sum << ", " << 100.0 * trash / elem_sum << "% точности" << endl;
        } else
        {
            cout << "Программе требуется разрешение на создание файлов и их редактирование.\n";
            cout << "В папке, где находится программа, запрещено создание файлов для Вашего пользователя.\n";
        }
    } else
    {
        cout << "Программа работает с файлом \"input.txt\" и выводит результат в \"output.txt\".\n";
        cout
                << "Если Вы видите это сообщение, файл \"input.txt\" не был найден, или у программы нет разрешения на чтение файла.\n";
        cout << "Образец форматирования файла можно найти в прилагаемом \"example.txt\"\n";
    }
    cout << "----------------------\n";
    in.close();
    out.close();
    system("pause");
    return 0;
}
