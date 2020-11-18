#include <stdio.h>
#include "TaskQueue.h"
#include "TasksPerformer.h"
#include <string>
#include <iostream>
#include <fstream>
#include <functional>
#include <vector>

using namespace std;

const int THREADS_COUNT = 8;
const string FILE_PATH = "test.txt";
typedef vector<string> Vector;

bool getFileContent(const string& fileName, Vector*& vectorStrings)
{
	ifstream dict_file(fileName);
	string line;

	if (!dict_file.good()) {
		return false;
	}

	while (getline(dict_file, line))
	{
		string new_line;
		new_line = line + "\n";

		if (new_line.size() != 0)
			vectorStrings->push_back(new_line);
	}

	return true;
}

void addVector(Vector* vect, TaskQueue queue) {
	queue.addTask([vect]() { sort(vect->begin(), vect->end()); });
}

Vector mergeTwo(const Vector& A, const Vector& B)
{
	size_t firstSize = A.size();
	size_t secondSize = B.size();

	Vector result;
	result.reserve(firstSize + secondSize);
	int i = 0, j = 0;
	while (i < firstSize && j < secondSize) {

		if (A[i] <= B[j])
		{
			result.push_back(A[i]);
			i++;
		}
		else
		{
			result.push_back(B[j]);
			j++;
		}
	}

	while (i < firstSize)
	{
		result.push_back(A[i]);
		i++;
	}

	while (j < secondSize)
	{
		result.push_back(B[j]);
		j++;
	}

	return result;
}

Vector mergeVectors(vector<vector<string>*>*& vectorParts) {
	Vector tmpVector;
	if (vectorParts->size() > 0) {
		tmpVector = *(*vectorParts)[0];
	}
	for (size_t i = 1; i < vectorParts->size(); i++) {
		tmpVector = mergeTwo(tmpVector, *(*vectorParts)[i]);
	}
	return tmpVector;
}

void outputVector(const Vector& vector) {
	for (auto str : vector) {
		printf("%s", str.c_str());
	}
}

void splitVectors(TaskQueue taskQueue, Vector*& vectorStrings, vector<Vector*>*& vectorParts, int threadsCount) {
	auto onePartCount = (int)floor((((double)vectorStrings->size()) / threadsCount) + .5);
	for (size_t i = 0; i < vectorStrings->size(); i += onePartCount) {
		auto* newVector = new vector<string>;
		vectorParts->push_back(newVector);
		for (size_t j = i; j < i + onePartCount; j++) {
			if (j < vectorStrings->size()) {
				string str = (*vectorStrings)[j];
				newVector->push_back(str);
			}
		}
		addVector(newVector, taskQueue);
	}
}

int main() {
	setlocale(LC_ALL, "Russian");
	auto* vectorStrings = new Vector();
	auto* vectorParts = new vector<Vector*>();
	bool result = getFileContent(FILE_PATH, vectorStrings);

	if (!result) {
		printf("Файл не существует.");
		vectorStrings->clear();
		vectorParts->clear();
		return -1;
	}

	TaskQueue taskQueue;
	TasksPerformer taskPerformer(taskQueue);
	int threadsCount = THREADS_COUNT > vectorStrings->size() ? (int)vectorStrings->size() : THREADS_COUNT;
	
	splitVectors(taskQueue, vectorStrings, vectorParts, threadsCount);
	taskPerformer.startPerform(threadsCount);

	Vector vector = mergeVectors(vectorParts);
	outputVector(vector);

	system("pause");
}