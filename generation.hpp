#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <string>
#include <iostream>

/* Variáveis da interface

nome_classe 		= nome da classe
nome_pacote			= nome do pacote
nome_metodo			= nome do método
params				= lista dos parâmetros
tipo_saida			= tipo da saída (retorno do método)
numero_conj_testes	= número de conjuntos de testes
tipos[t]			= todos os tipos

nome_conj_testes	= lista com os nomes de cada conjunto de teste
nro_testes_conj		= lista com número de testes de cada conjunto
saidas_esperadas	= lista com as strings concatenadas das saídas esperadas
intervalos_params	= lista com as strings concatenadas do intervalo de cada parâmetro para cada conjunto de teste
cc_bool				= se "caso contrário" for aplicável para tipo de saída boolean
*/

using namespace std;

void inicializa_arq_saida( string nome_classe, string nome_pacote );
void encerra_arq_saida();
//void gerar(string nome_metodo, string tipos[], string saida_esperada, vector<string> &intervalos_params, bool cc_bool, string tipo_saida, string nome_conj_testes, int cont, int nro_params);

static bool ehNumeroDecimal (string entrada);
int gera_letra();
int gera_todos();
void dominio_retorno(int inicio, int fim);
bool gera_valores_variavel(int variavel, int qtd);
void insere_testes_limitantes();
void insere_testes_classe_equivalencia( char* nome_testset, int cont_vars );

// funcoes random
static int random_int(int min, int max); //inclui o mínimo
double random_double(double min, double max);
int random_char();
int random_a_z();
int random_A_Z();

// variaveis para analise
FILE *arqOUTPUT;
string assert[16];
static bool nomes_conj_repetidos = false;

//inclui o mínimo
static int random_int(int min, int max) {
	//min++; // para nao incluir o minimo
    int range = (max+1 - min); 
    int div = RAND_MAX / range;
    return min + (rand() / div);
}

double random_double(double min, double max) {
    double range = (max - min); 
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

int random_char() {
    int range = (126 - 33); 
    int div = RAND_MAX / range;
    return 33 + (rand() / div);
}

int random_a_z() {
    int range = (122 - 97); 
    int div = RAND_MAX / range;
    return 97 + (rand() / div);
}

int random_A_Z() {
    int range = (90 - 65); 
    int div = RAND_MAX / range;
    return 65 + (rand() / div);
}

int gera_letra() {
    if(random_int(1,10)%2==0)
    	return random_a_z();
    else
	    return random_A_Z();
}

int gera_todos() {
	int num, num2;
	// 1 todos 33-47, 58-64, 91-96, 123-126, 48-57, 65-90, 97-122
	// 2 letras 65-90, 97-122
	// 3 numeros 48-57
	// 4 simbolos 33-47, 58-64, 91-96, 123-126 => exceções > 34("), 39('), 92(\)
	// 5 alfanumerico 65-90, 97-122, 48-57

	num = random_int(1,94);
	// int	possibilidades = 47-33+1+57-48+1+64-58+1+90-65+1+96-91+1+122-97+1+126-123+1;
	// printf("possibilidades = %d\n", possibilidades);

    if(num<=15) {
    	while(1) {
			num2 = random_int(33,47); // simbolos (15)
			if(num2 != 34 && num2 != 39 && num2 != 92) return num2;
    	}
	} else if(num<=25)
       	return random_int(48,57); // numeros (15+10=25)
	else if(num<=32)
       	return random_int(58,66); // + simbolos (25+7=32)
	else if(num<=58)
       	return random_int(65,90); // maiusculas (32+26=58)
	else if(num<=64) {
	    while(1) {
			num2 = random_int(91,96); // + simbolos (58+6=64)
			if(num2 != 92) return num2;
    	}
    } else if(num<=90)
       	return random_int(97,122); // minusculas (64+26=90)
    else if(num<=94)
       	return random_int(123,126); // + simbolos (90+4=94)
}

int gera_simbolos() {
	int num, num2;
	num = random_int(1,32);
	// int	possibilidades = 47-33+1+64-58+1+96-91+1+126-123+1;
	// printf("possibilidades = %d\n", possibilidades);
    if(num<=15) {
    	while(1) {
	    	num2 = random_int(33,47); // simbolos (15)
			if(num2 != 34 && num2 != 39) return num2;
		}
   	} else if(num<=22)
      	return random_int(58,64); // + simbolos (15+7=22)
    else if(num<=28) {
        while(1) {
	    	num2 = random_int(91,96); // + simbolos (22+6=28)
			if(num2 != 92) return num2;
		}
    } else if(num<=32)
      	return random_int(123,126); // + simbolos (28+4=32)	
}

int gera_numeros() {
   	return random_int(48,57); // numeros (0 a 9)
}

int gera_alfanumericos() {
	int num;
	num = random_int(1,62);
	// int	possibilidades = 57-48+1+90-65+1+122-97+1;
	// printf("possibilidades = %d\n", possibilidades);
    if(num<=10)
    	return random_int(48,57); // numeros (10)
   	else if(num<=36)
    	return random_int(65,90); // maiusculas (10+26=36)
    else if(num<=62)
    	return random_int(97,122); // minusculas (36+26=62)
}

static bool ehNumeroDecimal (string entrada) {
	for(int i=0; i<entrada.length(); i++)
		if(entrada[i] < 48 || entrada[i] > 57)
			if(!(entrada[i] == 46) && !(entrada[i] == 45)) { // ASCII: 44 = , / 45 = - / 46 = .
				//cout << entrada << " NÃO eh decimal" << endl;
				return false;
			}

	if(entrada[0]==46 || entrada[entrada.length()-1]==44 ) {
		//cout << entrada << " NÃO eh decimal" << endl;
		return false;
	}
	//cout << entrada << " eh decimal" << endl;	
	return true;
}

void inicializa_arq_saida( string nome_classe, string nome_pacote ) {
	char filename[50];
	strcpy(filename, nome_classe.c_str());
	strcat(filename, "Teste.java");
	// Abre arquivo texto para escrita. Caso nao exista cria, se ja existe recria
	arqOUTPUT = fopen(filename, "wt");

	// Informações do arquivo de testes (que está sendo gerado)
	fprintf(arqOUTPUT, "package %s;\n", nome_pacote.c_str());
	fprintf(arqOUTPUT, "import org.junit.*;\n");
	fprintf(arqOUTPUT, "import static org.junit.Assert.assertTrue;\n\n");
	fprintf(arqOUTPUT, "public class %sTeste extends %s {\n\n", nome_classe.c_str(), nome_classe.c_str());
}

void encerra_arq_saida() {	
	fprintf(arqOUTPUT, "\n}");
	fclose(arqOUTPUT);
}

static const vector <string> split(const string& s, const char& c) {
	string buff{""};
	vector<string> v;
	
	for(auto n:s) {
		if(n != c) buff+=n; else
		if(n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if(buff != "") v.push_back(buff);
	
	return v;
}

bool confere_colchetes(string str) {
	int abre,fecha; abre=0; fecha=0;
	for(int z=0; z<str.length(); z++) {
		if(str[z] == '[') abre++;
		if(str[z] == ']') fecha++;
	}
	if(abre != fecha || abre < 1) return false;
	else return true;
}

bool eh_intervalo_substr(string str) {
	for(int z=0; z<str.length(); z++)
		if(str[z] == '~') return true;
	return false;
}

string troca_virgula_por_ponto( string s ){
	for(int i=0; i<s.length(); i++)
		if (s[i] == ',')
			s[i] = '.';
	return s;
}

string string_fora_tamanho(string s) {
	vector <string> especificos; string ns; ns = ""; bool b; b = false;
	s.erase(remove(s.begin(), s.end(), ' '), s.end());
	s.pop_back();
	int num, acum_min, acum_max;
	acum_min=0; acum_max=0;
	
	s.erase(remove(s.begin(), s.end(), ' '), s.end());
	for(int i=0; i<s.length(); i++) {
		if(b) ns += s[i];
		if(s[i]=='&') b = true;
	}
	
	especificos = split(ns, ']');
	
	for(int i=0; i< especificos.size(); i++)
		especificos[i].erase( especificos[i].begin() );
	
	cout << "especificos: " << endl;
	for(int i=0; i<especificos.size(); i++)
		cout << especificos[i] << endl;		
}

bool gerar(string nome_metodo, string tipos[], string saida_esperada, vector<string> &intervalos_params, string tipo_saida, string nome_conj_testes, int cont, int nro_params, int conj) {

	string assert = "assertTrue(";
	string parcial, total;
	vector <string> se; // se = saídas esperadas;
	se = split(saida_esperada, '&');
	//for(int z=0; z<se.size(); z++)
	//	cout << se[z] << endl;
	if(tipo_saida=="boolean") {
	
		assert += " retorno == " + se[0] + " );";
	
	} else if(tipo_saida=="String") {
	
		if( confere_colchetes( se[0] ) && confere_colchetes( se[1] ) ) {
			vector <string> snpt1, snpt2;
			int tam; total = "";
			
			se[0].erase(remove(se[0].begin(), se[0].end(), ' '), se[0].end());
			se[1].erase(remove(se[1].begin(), se[1].end(), ' '), se[1].end());
			
			se[0].erase(remove(se[0].begin(), se[0].end(), '['), se[0].end());
			se[1].erase(remove(se[1].begin(), se[1].end(), '['), se[1].end());
			
			if( se[0].length() > 1 || se[1].length() > 1 ) {
				se[0].pop_back();
				se[1].pop_back();
			} else {
				cout << "String especificada incorretamente na saída esperada." << endl;
				return false;
			}
			
			snpt1 = split(se[0], ']'); // substrings
			snpt2 = split(se[1], ']'); // tamanhos substrings
			//cout << "snpt1.size(): " << snpt1.size() << endl;
			//for(int z=0; z<snpt1.size(); z++)
			//	cout << snpt1[z] << endl;
			//cout << "snpt2.size(): " << snpt2.size() << endl;
			//for(int z=0; z<snpt2.size(); z++)
			//	cout << snpt2[z] << endl;
			if( snpt1.size() == snpt2.size() ) {
				for(int i=0; i<snpt1.size(); i++) {
					//cout << "\nsnpt1[" << i << "]: " << snpt1[i] << endl;
					//cout << "snpt2[" << i << "]: " << snpt2[i] << "\n" << endl;
					if( snpt1[i]=="todos" ) {
											
						if(eh_intervalo_substr( snpt2[i] )) {
							vector <string> de_ate;
							int de, ate;
							de_ate = split(snpt2[i], '~');
							de=atoi(de_ate[0].c_str()); ate=atoi(de_ate[1].c_str());
							//cout << "De = " << de << " / Até = " << ate << endl;
							tam = random_int(de, ate); parcial = "";
							//cout << "tam = " << tam << endl;
							if(tam > 0)
								for(int w=0; w<tam; w++)
									parcial += gera_todos();
						} else {
							tam = atoi( snpt2[i].c_str() ); parcial = "";
							if(tam > 0)
								for(int w=0; w<tam; w++)
									parcial += gera_todos();
						}
						
					} else if( snpt1[i]=="letras" ) {
					
						if(eh_intervalo_substr( snpt2[i] )) {
							vector <string> de_ate;
							int de, ate;
							de_ate = split(snpt2[i], '~');
							de=atoi(de_ate[0].c_str()); ate=atoi(de_ate[1].c_str());
							
							tam = random_int(de, ate); parcial = "";
							if(tam > 0)
								for(int w=0; w<tam; w++)
									parcial += gera_letra();
						} else {
							tam = atoi( snpt2[i].c_str() ); parcial = "";
							if(tam > 0)
								for(int w=0; w<tam; w++)
									parcial += gera_letra();
						}
					
					} else if( snpt1[i]=="numeros" ) {
					
						if(eh_intervalo_substr( snpt2[i] )) {
							vector <string> de_ate;
							int de, ate;
							de_ate = split(snpt2[i], '~');
							de=atoi(de_ate[0].c_str()); ate=atoi(de_ate[1].c_str());
							
							tam = random_int(de, ate); parcial = "";
							if(tam > 0)
								for(int w=0; w<tam; w++)
									parcial += gera_numeros();
						} else {
							tam = atoi( snpt2[i].c_str() ); parcial = "";
							if(tam > 0)
								for(int w=0; w<tam; w++)
									parcial += gera_numeros();
						}
					
					} else if( snpt1[i]=="alfanumericos" ) {
					
						if(eh_intervalo_substr( snpt2[i] )) {
							vector <string> de_ate;
							int de, ate;
							de_ate = split(snpt2[i], '~');
							de=atoi(de_ate[0].c_str()); ate=atoi(de_ate[1].c_str());
							
							tam = random_int(de, ate); parcial = "";
							if(tam > 0)
								for(int w=0; w<tam; w++)
									parcial += gera_alfanumericos();
						} else {
							tam = atoi( snpt2[i].c_str() ); parcial = "";
							if(tam > 0)
								for(int w=0; w<tam; w++)
									parcial += gera_alfanumericos();
						}
					
					} else if( snpt1[i]=="simbolos" ) {
					
						if(eh_intervalo_substr( snpt2[i] )) {
							vector <string> de_ate;
							int de, ate;
							de_ate = split(snpt2[i], '~');
							de=atoi(de_ate[0].c_str()); ate=atoi(de_ate[1].c_str());
							
							tam = random_int(de, ate); parcial = "";
							if(tam > 0)
								for(int w=0; w<tam; w++)
									parcial += gera_simbolos();
						} else {
							tam = atoi( snpt2[i].c_str() ); parcial = "";
							if(tam > 0)
								for(int w=0; w<tam; w++)
									parcial += gera_simbolos();
						}
					
					} else {
						
						if(eh_intervalo_substr( snpt2[i] )) {
							vector <string> de_ate;
							int de, ate;
							de_ate = split(snpt2[i], '~');
							de=atoi(de_ate[0].c_str()); ate=atoi(de_ate[1].c_str());
							
							tam = random_int(de, ate); parcial = "";
							if(tam > 0)
								for(int w=0; w<tam; w++)
									parcial += snpt1[i];
						} else {
							tam = atoi( snpt2[i].c_str() ); parcial = "";
							if(tam > 0)
								for(int w=0; w<tam; w++)
									parcial += snpt1[i];
						}
					
					}
					total += parcial;
				}
			} else {
				cout << "String especificada incorretamente na saída esperada." << endl;
				cout << "O número de substrings é diferente da quantidade de tamanhos de substrings especificados" << endl;
				return false;
			}
		
		} else {
			cout << "String especificada incorretamente na saída esperada." << endl;
			cout << "Os colchetes não se correspondem." << endl;
			return false;
		}
		
		assert += " retorno == \"" + total + "\" );";
	
	} else if(tipo_saida=="char") {
		
		vector <string> caracts;
		saida_esperada.erase(remove(saida_esperada.begin(), saida_esperada.end(), ' '), saida_esperada.end());
		caracts = split(saida_esperada, ',');
		for(int i=0; i<caracts.size(); i++) {
			if( caracts[i].length() > 1 ) {
				cout << "Char especificado incorretamente na saída esperada." << endl;
				return false;
			} else {
				if(i==0)	assert += "retorno=='" + caracts[i] + "'";
				else		assert += " || retorno=='" + caracts[i] + "'";
			}
		}
		assert += ");";
		
	} else { // int/float/double
		
		if(se.size() == 1) { // apenas Incluir foi preenchido
			
			vector <string> especificos;
			saida_esperada.erase(remove(saida_esperada.begin(), saida_esperada.end(), ' '), saida_esperada.end());
			especificos = split(saida_esperada, ',');
			for(int i=0; i<especificos.size(); i++) {
				if(i==0)	assert += "retorno==" + especificos[i];
				else		assert += " || retorno==" + especificos[i];
			}
			assert += ");";
			
		} else if(se.size() == 2) { // De e Até preenchidos
		
			if(ehNumeroDecimal(se[0]) && ehNumeroDecimal(se[1])) {
				assert += "("+se[0]+" <= "+"retorno && retorno <= "+se[1]+"));";
			} else {
				cout << "Saída esperada (int/float/double) especificada incorretamente." << endl;
				cout << se[0] << " e/ou " << se[1] << " não é/são número(s)" << endl;
				return false;
			}	
		
		} else { // De, Até e Incluir preenchidos
			
			if(ehNumeroDecimal(se[0]) && ehNumeroDecimal(se[1]))
				assert += "("+se[0]+" <= "+"retorno && retorno <= "+se[1]+")";
			else {
				cout << "Saída esperada (int/float/double) especificada incorretamente." << endl;
				cout << se[0] << " e/ou " << se[1] << " não é/são número(s)" << endl;
				return false;
			}
			
			vector <string> especificos;
			se[2].erase(remove(se[2].begin(), se[2].end(), ' '), se[2].end());
			especificos = split(se[2], ',');
			for(int i=0; i<especificos.size(); i++)
				assert += " || (retorno==" + especificos[i]+")";
			assert += ");";
		}
	}
	// assert construído para ser adicionado ao final

	fprintf(arqOUTPUT, "\t@Test\n\t");
	fprintf(arqOUTPUT, "public void %s%d() {\n\t\t", nome_conj_testes.c_str(), cont);
	fprintf(arqOUTPUT, "%s retorno = %s(", tipo_saida.c_str(), nome_metodo.c_str());

	int pos; pos = conj * nro_params;
	// Inicia processo de geração dos parâmetros
	for(int z=0; z<nro_params; z++) {

		//cout << "intervalos_params[" << pos << "+" << z <<"]: " << intervalos_params[pos+z] << endl;
		vector <string> ip; // ip = intervalos params;
		ip.clear();
		ip = split(intervalos_params[pos+z], '&');
		
		if( tipos[z]=="boolean" ) {
			if(z==0) fprintf(arqOUTPUT, "%s", ip[0].c_str());
			else fprintf(arqOUTPUT, ", %s", ip[0].c_str());
		
		} else if( tipos[z]=="String" ) {
			
			if( confere_colchetes( ip[0] ) && confere_colchetes( ip[1] ) ) {
				vector <string> snpt1, snpt2;
				int tam; total = "";
				
				ip[0].erase(remove(ip[0].begin(), ip[0].end(), ' '), ip[0].end());
				ip[1].erase(remove(ip[1].begin(), ip[1].end(), ' '), ip[1].end());
				
				ip[0].erase(remove(ip[0].begin(), ip[0].end(), '['), ip[0].end());
				ip[1].erase(remove(ip[1].begin(), ip[1].end(), '['), ip[1].end());
				
				if( ip[0].length() > 1 || ip[1].length() > 1 ) {
					ip[0].pop_back();
					ip[1].pop_back();
				} else {
					cout << "String especificada incorretamente na saída esperada." << endl;
					return false;
				}
				
				snpt1 = split(ip[0], ']'); // substrings
				snpt2 = split(ip[1], ']'); // tamanhos substrings

				if( snpt1.size() == snpt2.size() ) {
					for(int i=0; i<snpt1.size(); i++) {

						if( snpt1[i]=="todos" ) {
												
							if(eh_intervalo_substr( snpt2[i] )) {
								vector <string> de_ate;
								int de, ate;
								de_ate = split(snpt2[i], '~');
								de=atoi(de_ate[0].c_str()); ate=atoi(de_ate[1].c_str());
								tam = random_int(de, ate); parcial = "";

								if(tam > 0)
									for(int w=0; w<tam; w++)
										parcial += gera_todos();
							} else {
								tam = atoi( snpt2[i].c_str() ); parcial = "";
								if(tam > 0)
									for(int w=0; w<tam; w++)
										parcial += gera_todos();
							}
							
						} else if( snpt1[i]=="letras" ) {
						
							if(eh_intervalo_substr( snpt2[i] )) {
								vector <string> de_ate;
								int de, ate;
								de_ate = split(snpt2[i], '~');
								de=atoi(de_ate[0].c_str()); ate=atoi(de_ate[1].c_str());
								
								tam = random_int(de, ate); parcial = "";
								if(tam > 0)
									for(int w=0; w<tam; w++)
										parcial += gera_letra();
							} else {
								tam = atoi( snpt2[i].c_str() ); parcial = "";
								if(tam > 0)
									for(int w=0; w<tam; w++)
										parcial += gera_letra();
							}
						
						} else if( snpt1[i]=="numeros" ) {
						
							if(eh_intervalo_substr( snpt2[i] )) {
								vector <string> de_ate;
								int de, ate;
								de_ate = split(snpt2[i], '~');
								de=atoi(de_ate[0].c_str()); ate=atoi(de_ate[1].c_str());
								
								tam = random_int(de, ate); parcial = "";
								if(tam > 0)
									for(int w=0; w<tam; w++)
										parcial += gera_numeros();
							} else {
								tam = atoi( snpt2[i].c_str() ); parcial = "";
								if(tam > 0)
									for(int w=0; w<tam; w++)
										parcial += gera_numeros();
							}
						
						} else if( snpt1[i]=="alfanumericos" ) {
						
							if(eh_intervalo_substr( snpt2[i] )) {
								vector <string> de_ate;
								int de, ate;
								de_ate = split(snpt2[i], '~');
								de=atoi(de_ate[0].c_str()); ate=atoi(de_ate[1].c_str());
								
								tam = random_int(de, ate); parcial = "";
								if(tam > 0)
									for(int w=0; w<tam; w++)
										parcial += gera_alfanumericos();
							} else {
								tam = atoi( snpt2[i].c_str() ); parcial = "";
								if(tam > 0)
									for(int w=0; w<tam; w++)
										parcial += gera_alfanumericos();
							}
						
						} else if( snpt1[i]=="simbolos" ) {
						
							if(eh_intervalo_substr( snpt2[i] )) {
								vector <string> de_ate;
								int de, ate;
								de_ate = split(snpt2[i], '~');
								de=atoi(de_ate[0].c_str()); ate=atoi(de_ate[1].c_str());
								
								tam = random_int(de, ate); parcial = "";
								if(tam > 0)
									for(int w=0; w<tam; w++)
										parcial += gera_simbolos();
							} else {
								tam = atoi( snpt2[i].c_str() ); parcial = "";
								if(tam > 0)
									for(int w=0; w<tam; w++)
										parcial += gera_simbolos();
							}
						
						} else {
							
							if(eh_intervalo_substr( snpt2[i] )) {
								vector <string> de_ate;
								int de, ate;
								de_ate = split(snpt2[i], '~');
								de=atoi(de_ate[0].c_str()); ate=atoi(de_ate[1].c_str());
								
								tam = random_int(de, ate); parcial = "";
								if(tam > 0)
									for(int w=0; w<tam; w++)
										parcial += snpt1[i];
							} else {
								tam = atoi( snpt2[i].c_str() ); parcial = "";
								if(tam > 0)
									for(int w=0; w<tam; w++)
										parcial += snpt1[i];
							}
						
						}
						total += parcial;
					}
				} else {
					cout << "String especificada incorretamente na saída esperada." << endl;
					cout << "O número de substrings é diferente da quantidade de tamanhos de substrings especificados" << endl;
					return false;
				}
			
			} else {
				cout << "String especificada incorretamente na saída esperada." << endl;
				cout << "Os colchetes não se correspondem." << endl;
				return false;
			}
			
			if(z==0) fprintf(arqOUTPUT, "\"%s\"", total.c_str());
			else fprintf(arqOUTPUT, ", \"%s\"", total.c_str());
			
		} else if( tipos[z]=="char" ) {
			vector <string> caracts;
			intervalos_params[pos+z].erase(remove(intervalos_params[pos+z].begin(), intervalos_params[pos+z].end(), ' '), intervalos_params[pos+z].end());
			caracts = split(intervalos_params[pos+z], ',');
			
			if( caracts.size() > 1 ) {
				int aleat,num; string c;
				aleat = random_int (0, caracts.size()-1);
				c = caracts[aleat];
				
				if( c=="todos" ) {
					num = gera_todos();
					if(z==0) fprintf(arqOUTPUT, "'%c'", num);
					else fprintf(arqOUTPUT, ", '%c'", num);
				} else if( c=="numeros" ) {
					num = gera_numeros();
					if(z==0) fprintf(arqOUTPUT, "'%c'", num);
					else fprintf(arqOUTPUT, ", '%c'", num);
				} else if( c=="letras" ) {
					num = gera_letra();
					if(z==0) fprintf(arqOUTPUT, "'%c'", num);
					else fprintf(arqOUTPUT, ", '%c'", num);
				} else if( c=="alfanumericos" ) {
					num = gera_alfanumericos();
					if(z==0) fprintf(arqOUTPUT, "'%c'", num);
					else fprintf(arqOUTPUT, ", '%c'", num);
				} else if( c=="simbolos" ) {
					num = gera_simbolos();
					if(z==0) fprintf(arqOUTPUT, "'%c'", num);
					else fprintf(arqOUTPUT, ", '%c'", num);
				} else if( c=="espaco" ) {
					if(z==0) fprintf(arqOUTPUT, "' '");
					else fprintf(arqOUTPUT, ", ' '");
				} else {
					if( c.length() > 1 ) {
						cout << "Char especificado incorretamente no intervalo de parâmetros." << endl;
						return false;
					} else {
						if(z==0) fprintf(arqOUTPUT, "'%c'", c.c_str());
						else fprintf(arqOUTPUT, ", '%c'", c.c_str());
					}
				}
				
			} else {
				string c; int num;
				c = caracts[0];
				
				if( c=="todos" ) {
					num = gera_todos();
					if(z==0) fprintf(arqOUTPUT, "'%c'", gera_todos());
					else fprintf(arqOUTPUT, ", '%c'", gera_todos());
				} else if( c=="numeros" ) {
					num = gera_numeros();
					if(z==0) fprintf(arqOUTPUT, "'%c'", gera_numeros());
					else fprintf(arqOUTPUT, ", '%c'", gera_numeros());
				} else if( c=="letras" ) {
					num = gera_letra();
					if(z==0) fprintf(arqOUTPUT, "'%c'", gera_letra());
					else fprintf(arqOUTPUT, ", '%c'", gera_letra());
				} else if( c=="alfanumericos" ) {
					num = gera_alfanumericos();
					if(z==0) fprintf(arqOUTPUT, "'%c'", gera_alfanumericos());
					else fprintf(arqOUTPUT, ", '%c'", gera_alfanumericos());
				} else if( c=="simbolos" ) {
					num = gera_simbolos();
					if(z==0) fprintf(arqOUTPUT, "'%c'", gera_simbolos());
					else fprintf(arqOUTPUT, ", '%c'", gera_simbolos());
				} else if( c=="vazio" ) {
					if(z==0) fprintf(arqOUTPUT, "''");
					else fprintf(arqOUTPUT, ", ''");
				} else {
					if( c.length() > 1 ) {
						cout << "Char especificado incorretamente no intervalo de parâmetros." << endl;
						return false;
					} else {
						if(z==0) fprintf(arqOUTPUT, "'%c'", c.c_str());
						else fprintf(arqOUTPUT, ", '%c'", c.c_str());
					}				
				}
			}
			
		} else if( tipos[z]=="int" ) { // int/float/double
		
			if(ip.size() == 1) { // apenas Incluir foi preenchido
			
				vector <string> especificos; int aleat;
				ip[0].erase(remove(ip[0].begin(), ip[0].end(), ' '), ip[0].end());
				especificos = split(ip[0], ',');
				
				if( especificos.size() == 1 ) {
					if(z==0) fprintf(arqOUTPUT, "%s", ip[0].c_str());
					else fprintf(arqOUTPUT, ", %s", ip[0].c_str());
				} else {
					aleat = random_int( 0, especificos.size()-1 );
					if(z==0) fprintf(arqOUTPUT, "%s", especificos[aleat].c_str());
					else fprintf(arqOUTPUT, ", %s", especificos[aleat].c_str());
				}
				
			} else if( ip.size() == 2 ) {// De e Até preenchidos
			
				if(ehNumeroDecimal(ip[0]) && ehNumeroDecimal(ip[1])) {

					if(cont == 1) { // Testa limite(s) inferior(es)
						if(z==0) fprintf(arqOUTPUT, "%s", ip[0].c_str());
						else fprintf(arqOUTPUT, ", %s", ip[0].c_str());
					} else if(cont == 2) { // Testa limite(s) superior(es)
						if(z==0) fprintf(arqOUTPUT, "%s", ip[1].c_str());
						else fprintf(arqOUTPUT, ", %s", ip[1].c_str());
					} else {
						int num; num = random_int( atoi(ip[0].c_str()),atoi(ip[1].c_str()) );
						if(z==0) fprintf(arqOUTPUT, "%d", num);
						else fprintf(arqOUTPUT, ", %d", num);
					}
					
				} else {
					cout << "Saída esperada (int) especificada incorretamente." << endl;
					cout << ip[0] << " e/ou " << ip[1] << " não é/são número(s)" << endl;
					return false;
				}
			
			} else if( ip.size() == 3 ) {// De, Até e Incluir preenchidos
			
				vector <string> especificos; int qtd_de_ate, qtd_incluir, aleat;
				qtd_de_ate = atoi(ip[1].c_str()) - atoi(ip[0].c_str()) + 1;
				
				ip[2].erase(remove(ip[2].begin(), ip[2].end(), ' '), ip[2].end());
				especificos = split(ip[2], ',');
				qtd_incluir = especificos.size();
				
				aleat = random_int(0, qtd_de_ate + qtd_incluir - 1);

				if(cont == 1) { // Testa limite(s) inferior(es)
					if(z==0) fprintf(arqOUTPUT, "%s", ip[0].c_str());
					else fprintf(arqOUTPUT, ", %s", ip[0].c_str());
				} else if(cont == 2) { // Testa limite(s) superior(es)
					if(z==0) fprintf(arqOUTPUT, "%s", ip[1].c_str());
					else fprintf(arqOUTPUT, ", %s", ip[1].c_str());
				} else {
					if(aleat < qtd_incluir) {// insere valor do incluir
						aleat = random_int(0, especificos.size()-1);
						if(z==0) fprintf(arqOUTPUT, "%s", especificos[aleat].c_str());
						else fprintf(arqOUTPUT, ", %s", especificos[aleat].c_str());

					} else {// insere um valor do intervalo "de-ate"
					
						aleat = random_int( atoi(ip[0].c_str()), atoi(ip[1].c_str()) );
						if(z==0) fprintf(arqOUTPUT, "%d", aleat);
						else fprintf(arqOUTPUT, ", %d", aleat);
					}
				}
			}
			
		} else { // float/double
		
			if(ip.size() == 1) {// apenas Incluir foi preenchido
			
				vector <string> especificos; int aleat;
				ip[0].erase(remove(ip[0].begin(), ip[0].end(), ' '), ip[0].end());
				especificos = split(ip[0], ',');
				
				if( especificos.size() == 1 ) {
					if(z==0) fprintf(arqOUTPUT, "%s", ip[0].c_str());
					else fprintf(arqOUTPUT, ", %s", ip[0].c_str());
				} else {
					aleat = random_int( 0, especificos.size()-1 );
					if(z==0) fprintf(arqOUTPUT, "%s", especificos[aleat].c_str());
					else fprintf(arqOUTPUT, ", %s", especificos[aleat].c_str());
				}
				
			} else if( ip.size() == 2 ) {// De e Até preenchidos

				if(ehNumeroDecimal(ip[0]) && ehNumeroDecimal(ip[1])) {
					double a, b; int casas_decimais; string result;
					//cout << "ip[0]: " << ip[0] << endl;
					//cout << "ip[1]: " << ip[1] << endl;
					a = atof(ip[0].c_str());
					b = atof(ip[1].c_str());

					if(cont == 1) { // Testa limite(s) inferior(es)
						if(z==0) fprintf(arqOUTPUT, "%s", ip[0].c_str());
						else fprintf(arqOUTPUT, ", %s", ip[0].c_str());
					} else if(cont == 2) { // Testa limite(s) superior(es)
						if(z==0) fprintf(arqOUTPUT, "%s", ip[1].c_str());
						else fprintf(arqOUTPUT, ", %s", ip[1].c_str());
					} else {
						
						double num; num = random_double( a,b );
						result = to_string(num); result = troca_virgula_por_ponto( result );
						
						if(tipos[z]=="float")  casas_decimais = random_int(1,3);
						if(tipos[z]=="double") casas_decimais = random_int(3,6);
						
						if(cont == 1) { // Testa limite(s) inferior(es)
							if(z==0) fprintf(arqOUTPUT, "%s", ip[0].c_str());
							else fprintf(arqOUTPUT, ", %s", ip[0].c_str());
						} else if(cont == 2) { // Testa limite(s) superior(es)
							if(z==0) fprintf(arqOUTPUT, "%s", ip[1].c_str());
							else fprintf(arqOUTPUT, ", %s", ip[1].c_str());
						} else {
							if(casas_decimais < 6)
								for(int n=0; n<6-casas_decimais; n++)
									result.pop_back();

							if(ip[0].front()!='-' && ip[1].front()=='-') {
								cout << "Saída esperada (%s) especificada incorretamente." << tipos[z].c_str() << endl;
								cout << ip[0] << " deve ser menor que " << ip[1] << "." << endl;
								return false;
							}
						}
						
						if(z==0) fprintf(arqOUTPUT, "%s", result.c_str());
						else fprintf(arqOUTPUT, ", %s", result.c_str());
					}
					
				} else {
					cout << "Saída esperada (%s) especificada incorretamente." << tipos[z].c_str() << endl;
					cout << ip[0] << " e/ou " << ip[1] << " não é/são número(s)" << endl;
					return false;
				}
			
			} else if( ip.size() == 3 ) {// De, Até e Incluir preenchidos
			
				vector <string> especificos; int qtd_de_ate, qtd_incluir, aleat; double valor;
				qtd_de_ate = (int)atoi(ip[1].c_str()) - (int)atoi(ip[0].c_str()) + 1;
				
				ip[2].erase(remove(ip[2].begin(), ip[2].end(), ' '), ip[2].end());
				especificos = split(ip[2], ',');
				qtd_incluir = especificos.size();
				
				aleat = random_int(0, qtd_de_ate + qtd_incluir - 1);
				
				if(aleat < qtd_incluir) {// insere valor do incluir
					valor = random_double(0, especificos.size()-1);
					if(z==0) fprintf(arqOUTPUT, "%s", especificos[aleat].c_str());
					else fprintf(arqOUTPUT, ", %s", especificos[aleat].c_str());

				} else {// insere um valor do intervalo "de-ate"
				
					if(ehNumeroDecimal(ip[0]) && ehNumeroDecimal(ip[1])) {
						double a, b; int casas_decimais; string result;
						//cout << "ip[0]: " << ip[0] << endl;
						//cout << "ip[1]: " << ip[1] << endl;
						a = atof(ip[0].c_str());
						b = atof(ip[1].c_str());

						if(cont == 1) { // Testa limite(s) inferior(es)
							if(z==0) fprintf(arqOUTPUT, "%s", ip[0].c_str());
							else fprintf(arqOUTPUT, ", %s", ip[0].c_str());
						} else if(cont == 2) { // Testa limite(s) superior(es)
							if(z==0) fprintf(arqOUTPUT, "%s", ip[1].c_str());
							else fprintf(arqOUTPUT, ", %s", ip[1].c_str());
						} else {
							
							double num; num = random_double( a,b );
							result = to_string(num); result = troca_virgula_por_ponto( result );
							
							if(tipos[z]=="float")  casas_decimais = random_int(1,3);
							if(tipos[z]=="double") casas_decimais = random_int(3,6);
							
							if(casas_decimais < 6)
								for(int n=0; n<6-casas_decimais; n++)
									result.pop_back();

							if(ip[0].front()!='-' && ip[1].front()=='-') {
								cout << "Saída esperada (%s) especificada incorretamente." << tipos[z].c_str() << endl;
								cout << ip[0] << " deve ser menor que " << ip[1] << "." << endl;
								return false;
							}
							
							if(z==0) fprintf(arqOUTPUT, "%s", result.c_str());
							else fprintf(arqOUTPUT, ", %s", result.c_str());
						}
						
					} else {
						cout << "Saída esperada (%s) especificada incorretamente." << tipos[z].c_str() << endl;
						cout << ip[0] << " e/ou " << ip[1] << " não é/são número(s)" << endl;
						return false;
					}
				}
			}
		}
	}
	
	fprintf(arqOUTPUT, ");");
	fprintf(arqOUTPUT, "\n\t\t%s\n\t}\n\n", assert.c_str());
	return true;
}

