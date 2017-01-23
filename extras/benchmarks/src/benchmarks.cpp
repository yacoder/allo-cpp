#include <chrono>
#include <atomic>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>
#include <thread>
#include <unordered_map>
#include <vector>

#include "allo.hpp"

using namespace std;
using namespace std::chrono;

namespace
{

template <typename Container> int mostFrequentNumber(const vector<int>& input, Container& map)
{
   int max_count = 0;
   int max_number = 0;

   for (const auto& number : input)
   {
      // Emplaces {number, 0} pair if the number is not yet in the map.
      // Returns pair {iterator, bool}.
      auto emplace_result = map.try_emplace(number, 0);

      // Increase the counter of occurences in the map.
      auto count = ++(emplace_result.first->second);

      if (count > max_count)
      {
         max_count = count;
         max_number = number;
      }
   }

   return max_number;
}

int mostFrequentNumberStdMap(const vector<int>& input) { return mostFrequentNumber(input, std::map<int, int>()); }

int mostFrequentNumberAlloMap(const vector<int>& input)
{
   allo::containers::wrapped_map<int, int> wrapped_map(500'000);
   auto& map = wrapped_map.unwrap();
   return mostFrequentNumber(input, map);
}

template <typename MostFrequentNumberImplementation>
pair<milliseconds, vector<int>> benchmark_most_frequent_numbers_iteration(int number_of_threads,
                                                                          vector<vector<int>>& inputs,
                                                                          const MostFrequentNumberImplementation& f)
{
   vector<int> outputs(inputs.size());

   auto tBegin = steady_clock::now();

   vector<thread> threads;
   for (int thread_number = 0; thread_number < number_of_threads; thread_number++)
   {
       auto workItem = [&inputs, &f, number_of_threads, &outputs, thread_number] {

           size_t range_size = inputs.size() / number_of_threads;
           size_t range_begin = range_size*thread_number;
           size_t range_end = range_size*(thread_number + 1);

           for (size_t i = range_begin; i < inputs.size() && i < range_end; i++)
           {
               outputs[i] = f(inputs[i]);
           }
       };

      threads.emplace_back(workItem);
   }

   for (auto& thread : threads)
   {
      thread.join();
   }

   auto tEnd = steady_clock::now();

   return {duration_cast<milliseconds>(tEnd - tBegin), std::move(outputs)};
}

void benchmark_most_frequent_numbers()
{
   const int n_inputs = 8'000;
   const int n_elements_per_input = 10'000;
   const int random_seed = 42;

   vector<vector<int>> inputs;

   mt19937 e1(random_seed);
   uniform_int_distribution<int> uniform_dist(1, n_elements_per_input);

   for (int i = 0; i < n_inputs; i++)
   {
      vector<int> v;
      v.reserve(n_elements_per_input);

      for (size_t i = 0; i < n_elements_per_input; i++)
      {
         v.push_back(uniform_dist(e1));
      }

      inputs.push_back(move(v));
   }

   cout << "Random inputs generated, starting benchmark." << endl;

   ofstream fout;
   fout.open("benchmark_most_frequent_numbers.csv", ios::out);

   fout << "Number of threads,\"std::map time, milliseconds\",\"allo::map time, milliseconds\"" << endl;
   cout << "threads\t\tstd time\tallo time" << endl;

   for (int number_of_threads = 1; number_of_threads <= 4; number_of_threads++)
   {
      milliseconds stdTime, alloTime;
      vector<int> stdOutput, alloOutput;

      std::tie(stdTime, stdOutput) =
          benchmark_most_frequent_numbers_iteration(number_of_threads, inputs, mostFrequentNumberStdMap);
      std::tie(alloTime, alloOutput) =
          benchmark_most_frequent_numbers_iteration(number_of_threads, inputs, mostFrequentNumberAlloMap);

      if (stdOutput != alloOutput)
      {
         cerr << "Oops! Different results returned for n_threads=" << number_of_threads << endl;
         fout << "Oops! Different results returned for n_threads=" << number_of_threads << endl;
         std::terminate();
      }

      cout << number_of_threads << "\t\t" << stdTime.count() << "ms\t\t" << alloTime.count() << "ms" << endl;
      fout << number_of_threads << "," << stdTime.count() << "," << alloTime.count() << endl;
   }
}

} // end of unnamed namespace

int main()
{
   benchmark_most_frequent_numbers();

   return 0;
}