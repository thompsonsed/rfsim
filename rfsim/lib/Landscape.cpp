/**
 * @brief Contains the main Landscape class which controls the simulation.
 */

#include "Landscape.h"
#include "numpy/arrayobject.h"

void capsuleCleanup(PyObject* capsule)
{
    void* memory = PyCapsule_GetPointer(capsule, NULL);
    // Use your specific gc implementation in place of free if you have to
    free(memory);
}

void Landscape::setSeed(unsigned long i)
{
    random->setSeed(i);
}

void Landscape::iterate()
{
    vector<Rabbit> moved_rabbits;
    vector<Fox> moved_foxes;
    for(unsigned long i = 0; i < landscape.getRows(); i++)
    {
        for(unsigned long j = 0; j < landscape.getCols(); j++)
        {
            Cell cell = landscape.get(i, j);
            cell.growGrass();
            cell.iterate(random);
            auto tmp_rabbits = cell.moveRabbits(random, landscape.getCols(), landscape.getRows());
            auto tmp_foxes = cell.moveFoxes(random, landscape.getCols(), landscape.getRows());
            //                moved_rabbits.reserve(moved_rabbits.size() + distance(tmp_rabbits.begin(), tmp_rabbits.end()));
            //                moved_foxes.reserve(moved_foxes.size() + distance(tmp_foxes.begin(), tmp_foxes.end()));
            moved_rabbits.insert(moved_rabbits.end(), tmp_rabbits.begin(), tmp_rabbits.end());
            moved_foxes.insert(moved_foxes.end(), tmp_foxes.begin(), tmp_foxes.end());
        }
    }
    // Now move all the moved rabbits and foxes
    for(auto &rabbit: moved_rabbits)
    {
        Coordinates new_location = rabbit.getLocation();
        landscape.get(new_location.y, new_location.x).addRabbit(rabbit);
    }
    for(auto &fox: moved_foxes)
    {
        Coordinates new_location = fox.getLocation();
        landscape.get(new_location.y, new_location.x).addFox(fox);
    }
    // Kill rabbits back down to some arbitary carrying capacity.
    for(unsigned long i = 0; i < landscape.getRows(); i++)
    {
        for(unsigned long j = 0; j < landscape.getCols(); j++)
        {
            landscape.get(i, j).checkRabbitCarryingCapacity(100);
            landscape.get(i, j).checkFoxCarryingCapacity(10);
        }
    }
}

void Landscape::setLandscapeSize(unsigned long x_size, unsigned long y_size)
{
    landscape.setSize(y_size, x_size);
    for(unsigned long i = 0; i < y_size; i++)
    {
        for(unsigned long j = 0; j < x_size; j++)
        {
            Coordinates tmp_coordinate = Coordinates(j, i);
            landscape.get(i, j).setLocation(tmp_coordinate);
        }
    }
}

void Landscape::print()
{
    for(unsigned long i = 0; i < landscape.getRows(); i++)
    {
        for(unsigned long j = 0; j < landscape.getCols(); j++)
        {
            std::cout << "||" << landscape.get(j, i).getNumRabbits() << "|" << landscape.get(j, i).getNumFoxes();
        }
        std::cout << std::endl;
    }
}

PyObject* Landscape::getRabbitNumbers()
{
    // this is required for numpy
    import_array1(nullptr);
    // Dimensions of the numpy array
    npy_intp dims[2]{static_cast<long int>(landscape.getRows()), static_cast<long int>(landscape.getCols())};
    // the output data
    int* out_rabbits = new int[landscape.getRows()*landscape.getCols()];
    unsigned long index = 0;
    for(unsigned long i = 0; i < landscape.getRows(); i++)
    {
        for(unsigned long j = 0; j < landscape.getCols(); j++)
        {
            out_rabbits[index] = static_cast<int>(landscape.get(i, j).getNumRabbits());
            index++;
        }
    }
    PyObject* pArray = PyArray_SimpleNewFromData(2, dims, NPY_INT, (void*) out_rabbits);
    PyObject* capsule = PyCapsule_New(out_rabbits, NULL, capsuleCleanup);
    // NULL can be a string but use the same string while calling PyCapsule_GetPointer inside capsule_cleanup
    PyArray_SetBaseObject((PyArrayObject*) pArray, capsule);
    return pArray;
}

PyObject* Landscape::getFoxNumbers()
{
    // this is required for numpy
    import_array1(nullptr);
    // Dimensions of the numpy array
    npy_intp dims[2]{static_cast<long int>(landscape.getRows()), static_cast<long int>(landscape.getCols())};
    // the output data
    int* out_foxes = new int[landscape.getRows() * landscape.getCols()];
    unsigned long index = 0;
    for(unsigned long i = 0; i < landscape.getRows(); i++)
    {
        for(unsigned long j = 0; j < landscape.getCols(); j++)
        {
            out_foxes[index] = static_cast<int>(landscape.get(i, j).getNumFoxes());
            index++;
        }
    }
    PyObject* pArray = PyArray_SimpleNewFromData(2, dims, NPY_INT, (void*) out_foxes);
    PyObject* capsule = PyCapsule_New(out_foxes, NULL, capsuleCleanup);
    // NULL can be a string but use the same string while calling PyCapsule_GetPointer inside capsule_cleanup
    PyArray_SetBaseObject((PyArrayObject*) pArray, capsule);
    return pArray;
}
