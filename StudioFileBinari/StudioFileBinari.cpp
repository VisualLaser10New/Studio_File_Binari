#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
using namespace std;

namespace FPersone
{
	class Residenza
	{
	public:
		int civico, metriquadri;
		string indirizzo;

		Residenza() = default;
		Residenza(int nCivico, int metriquadri, string indirizzo)
		{
			this->civico = nCivico;
			this->metriquadri = metriquadri;
			this->indirizzo = indirizzo;
		}
	};

	class Persona
	{
	public:
		int altezza, peso;
		string nome, cognome;
		vector<string> mansioni;
		vector<Residenza> villeggiature;

		Persona() = default;
		Persona(int alt, int peso, string nome, string cognome, vector<string> lavori, vector<Residenza> villeggiature)
		{
			this->altezza = alt;
			this->peso = peso;
			this->nome = nome;
			this->cognome = cognome;

			this->mansioni = lavori;
			this->villeggiature = villeggiature;
		}
	};

	namespace FString
	{
		void saveString(fstream& file, string input)
		{
			size_t len = input.size();
			file.write((char*)&len, sizeof(size_t));
			file << input;
		}

		string readString(fstream& file)
		{
			size_t len = 0;
			string output;

			file.read((char*)&len, sizeof(size_t));
			vector<char> caratt(len);
			file.read(caratt.data(), len);
			output.assign(caratt.data(), len);
			return output;
		}

		void saveVectorString(fstream& file, vector<string> input)
		{
			size_t lenArr = input.size();
			file.write((char*)&lenArr, sizeof(size_t));

			for (auto i : input)
			{
				saveString(file, i);
			}
		}

		vector<string> readVectorString(fstream& file)
		{
			size_t lenArr = 0;
			vector<string> output;

			file.read((char*)&lenArr, sizeof(size_t));
			for (auto i = 0; i < lenArr; i++)
			{
				output.push_back(readString(file));
			}
			return output;
		}

	}

	namespace FResidenza
	{
		void saveResidenza(fstream& file, Residenza input)
		{
			file.write((char*)&input.civico, sizeof(int));
			file.write((char*)&input.metriquadri, sizeof(int));
			FString::saveString(file, input.indirizzo);
		}

		Residenza readResidenza(fstream& file)
		{
			Residenza output;

			file.read((char*)&output.civico, sizeof(int));
			file.read((char*)&output.metriquadri, sizeof(int));
			output.indirizzo = FString::readString(file);

			return output;
		}

		void saveVectorResidenza(fstream& file, vector<Residenza> input)
		{
			size_t lenArr = input.size();
			file.write((char*)&lenArr, sizeof(size_t));

			for (auto i : input)
			{
				saveResidenza(file, i);
			}
		}

		vector<Residenza> readVectorResidenza(fstream& file)
		{
			size_t lenArr = 0;
			file.read((char*)&lenArr, sizeof(size_t));

			vector<Residenza> output;
			for (auto i = 0; i < lenArr; i++)
			{
				output.push_back(readResidenza(file));
			}
			return output;
		}
	}

	namespace FPersona
	{
		void SalvaPersona(fstream& file, Persona input)
		{
			file.write(reinterpret_cast<char*>(&input.altezza), sizeof(int));
			file.write(reinterpret_cast<char*>(&input.peso), sizeof(int));

			FString::saveString(file, input.nome);
			FString::saveString(file, input.cognome);

			FString::saveVectorString(file, input.mansioni);
			FResidenza::saveVectorResidenza(file, input.villeggiature);
		}

		Persona LeggiPersona(fstream& file)
		{
			Persona output;
			file.read((char*)&output.altezza, sizeof(int));
			file.read((char*)&output.peso, sizeof(int));

			output.nome = FString::readString(file);
			output.cognome = FString::readString(file);

			output.mansioni = FString::readVectorString(file);
			output.villeggiature = FResidenza::readVectorResidenza(file);

			return output;
		}

		void SalvaVectorPersone(fstream& file, vector<Persona> persone)
		{
			size_t lenArr = persone.size();
			file.write((char*)&lenArr, sizeof(size_t));	//scrittura del lunghezza del vettore

			for (auto pers : persone)
			{
				FPersona::SalvaPersona(file, pers);			//salvataggio della persona
			}
		}

		vector<Persona> LeggiVectorPersone(fstream& file)
		{
			size_t lenArr = 0;
			file.read((char*)&lenArr, sizeof(size_t));

			vector<Persona> output;

			for (auto i = 0; i < lenArr; i++)
			{
				output.push_back(FPersona::LeggiPersona(file));
			}
			return output;
		}
	}
}

static vector<FPersone::Persona> persone;

void fillVector(int howmany)
{
	int n = 1;
	for (int i = 0; i < howmany; i++)
	{
		vector<FPersone::Residenza> vecResidenze;
		for (int j = 0; j < (i + 4); j++)
		{
			vecResidenze.push_back(FPersone::Residenza{ (n * j) / 3, (n * j) / 2, "via gino " + to_string((n * i) + 5) });
		}

		vector<string> mansioni;
		for (int j = 0; j < ((i+1) * 4); j++)
		{
			mansioni.push_back(string("lavoro " + to_string(j*i)));
		}
		
		persone.push_back(FPersone::Persona{ n * i + 1, n * i + 2, "Rastaldo " + to_string(n * i + 1), "Baletti " + to_string(n * i + 2), mansioni, vecResidenze });
	}
}

void printVector()
{
	for (auto pers : persone)
	{
		cout << "Altezza: " << pers.altezza << "\nPeso: " << pers.peso << 
			"\nNome: "  << pers.nome << "\nCognome: " << pers.cognome << endl;

		cout << "Mansioni: ";
		for (auto m : pers.mansioni)
		{
			cout << m << "; ";
		}

		cout << "\n\nVilleggiature:" << endl;
		for (auto v : pers.villeggiature)
		{
			cout << "\nIndirizzo: " << v.indirizzo << " " << v.civico <<
				"\n  Metratura: " << v.metriquadri << endl;
		}

		cout << "\n================================\n" << endl;
	}
}

int salvataggio(string filenames)
{
	fillVector(3);

	cout << "Prima del file" << endl;
	printVector();

	system("pause");

	//SALVATAGGIO DEL VETTORE
	fstream file(filenames, ios::binary | ios::out);

	if (!file)
	{
		cout << "Il file non \x8a stato salvato" << endl;
		system("pause");
		return 1;
	}

	FPersone::FPersona::SalvaVectorPersone(file, persone);
	file.close();

	cout << "File salvato in " << filenames << endl;

	system("pause");

}

int lettura(string filenames)
{
	//LETTURA DEL VETTORE
	fstream filer(filenames, ios::binary | ios::in);

	if (!filer)
	{
		cout << "Il file non \x8a stato aperto" << endl;
		system("pause");
		return 1;
	}

	persone = FPersone::FPersona::LeggiVectorPersone(filer);
	cout << "File aperto e letto" << endl;
	filer.close();

	system("pause");

	printVector();

	system("pause");
}

int main()
{
	string filenames = "persone.dati";
	
	//scrittura
	salvataggio(filenames);

	persone.clear();
	cout << "Vettore Cancellato" << endl;
	printVector();
	system("pause");
	
	//lettura
	lettura(filenames);

	return 0;
}
