#include <iostream>
#include <fstream>
#include <unistd.h>
#include <wait.h>
#include <vector>
#include <sys/types.h>
#include <cstdlib>
#include <string>

using namespace std;

void works(int n, int min, int max)
{
	srand(time(0));
	ofstream four("tasks.txt");
	if (!four.is_open())
	{
		cout << "Can't create file" << endl;
		exit(1);
	}
	for (int i = 0; i < n; i++)
	{
		four << "task" + to_string(i) << ' ' << min + rand() % (max - min + 1) << endl;
	}
	four.close();
}
vector<double> read_file(int n)
{
	string temp;
	double tempnum;
	int g = 0;
	vector<double> time(n);
	ifstream fin("tasks.txt");
	if (!fin.is_open())
	{
		cout << "Can't open file" << endl;
		exit(2);
	}
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if (j == 1)
			{
				fin >> tempnum;
				time[g] = (tempnum);
				g++;
			}
			else
			{
				fin >> temp;
			}
		}
	}
	fin.close();
	return time;
}
void distribution(int p, int n)
{
	vector <double> tasks = read_file(n);
	int parent[2];
	int child[2];
	int empty = -1;
	int n_temp = 0;
	pid_t Parent = getpid();
	pid_t children[p];
	pipe(parent);
	pipe(child);
	for (int i = 0; i < p; i++)
	{
		if (Parent == getpid())
		{
			children[i] = fork();
		}
		else
		{
			break;
		}
	}
	if (Parent == getpid())
	{
		close(parent[0]);
		close(child[1]);
		int tempP;
		for (int g = 0; g < p; g++)
		{
			cout << "Gave the task ¹" << n_temp << "  PID:" << getpid() << endl;
			write(parent[1], &n_temp, sizeof(int));
			n_temp++;
		}
		while (read(child[0], &tempP, sizeof(int)))
		{
			if (tempP == -1)
			{
				if (n_temp != n)
				{
					cout << "Gave the task ¹" << n_temp << "  PID:" << getpid() << endl;
					write(parent[1], &n_temp, sizeof(int));
					n_temp++;
				}
				else
				{
					for (int g = 0; g < p; g++)
					{
						write(parent[1], &empty, sizeof(int));
					}
					break;
				}
			}
		}
		close(parent[1]);
		close(child[0]);
	}
	else
	{
		int tempC;
		close(parent[1]);
		close(child[0]);
		while (read(parent[0], &tempC, sizeof(int)))
		{
			if (tempC != empty)
			{
				cout << "Accepted the task ¹" << tempC << "  PID:" << getpid() << "  Time:" << tasks[tempC] << endl;
				sleep(tasks[tempC] / 10.);
				cout << "Task ¹" << tempC << " completed!" << "  PID:" << getpid() << endl;
				write(child[1], &empty, sizeof(int));
			}
			else
			{
				break;
			}
		}
		close(parent[0]);
		close(child[1]);

	}
	if (Parent == getpid())
	{
		for (int j = 0; j < p; j++)
		{
			wait(&children[j]);
		}
	}

}
int main(int argc, char* argv[])
{
	int p;
	int n;
	int min;
	int max;
	for (int i = 1; i < argc; i++)
	{
		if (string(argv[i]) == "--n")
		{
			n = atoi(argv[i + 1]);
		}
		else if (string(argv[i]) == "--min")
		{
			min = atoi(argv[i + 1]);
		}
		else if (string(argv[i]) == "--max")
		{
			max = atoi(argv[i + 1]);
		}
		else if (string(argv[i]) == "--p")
		{
			p = atoi(argv[i + 1]);
		}
	}
	works(n, min, max);
	distribution(p, n);
}
