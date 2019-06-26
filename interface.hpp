#include <gtk/gtk.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <sstream>
#include <string.h>
#include <string>
#include <iostream>
#include <algorithm>

#include "generation.hpp"

using namespace std;

static void entry_page1_changed (GtkEditable*, GtkAssistant*);
static void assistant_cancel (GtkAssistant*, gpointer);
static void assistant_close  (GtkAssistant*, gpointer);
static void constroi_pagina0();

static bool ehNumeroInteiroDiferenteDeZero (string entrada);
static bool ehNumeroInteiro (string entrada);

static void on_changed_combobox (GtkComboBox *widget);
static void visualizador_dados_entrada(GtkWidget *p_widget);
static void criar_assistente_info_gerais (GtkWidget *widget);
static void criar_assistente_tipos (GtkWidget *widget);
static void definir_conjuntos_testes(GtkWidget *p_widget);
static void mostrar_sobre(GtkWidget *widget);
static const vector <string> split(const string& s, const char& c);
static void gerar_testes(GtkWidget *p_widget);
static bool entradaTamanhoString (string entrada);

static void definir_conjuntos_testes(GtkWidget *p_widget);
static void cabecalho_especificao(GtkWidget *p_widget);
static void corpo_especificao(GtkWidget *p_widget);

struct {
	gchar *text;
	void  (*func) (GtkWidget *widget);
} buttons[] =
	{
		{ "Visualizar dados de entrada", visualizador_dados_entrada },
		{ "Definir informações do método", criar_assistente_info_gerais },
		{ "Definir tipos dos parâmetros", criar_assistente_tipos },
		{ "Especificar conjuntos de teste", definir_conjuntos_testes },
		{ "Gerar testes", gerar_testes },
		{ "Sobre o AutomTest", mostrar_sobre },
	};

// variaveis globais da interface gráfica
GtkWidget *window_root, *button_confirmar, *label_pagina, *toggle;
static gboolean cc_bool = FALSE;

// entradas para assistente 1 (informações do método)
GtkWidget *entry_nome_classe;
GtkWidget *entry_nome_pacote;
GtkWidget *entry_nome_metodo;
GtkWidget *entry_params;
GtkWidget *tipo_pag1_combo_box;
GtkWidget *entry_num_conj_testes;


// entradas para assistente 2 (tipos do parâmetros)
GtkWidget *combo_box_tipo[36];


// entradas para assistente 3 (intervalos dos parâmetros para cada conjunto de testes)
GtkWidget *entry_nome_conj_testes;	// nome dos conjuntos de testes
GtkWidget *entry_nro_testes;		// numero de testes dos conjuntos de testes
	// saídas esperadas
	GtkWidget *combobox_boolean;
	GtkWidget *entry_aux1, *entry_aux2, *entry_aux3, *entry_aux4;


// variaveis globais para processo de geração de teste
int numero_conj_testes = 0, qtd_params;
vector <string> params;
string tipos[16];
string nome_metodo, nome_classe, nome_pacote, parametros;
vector <string> nome_conj_testes, nro_testes_conj, saidas_esperadas, intervalos_params;
vector <string> nome_conj_testes_cc, saidas_esperadas_cc, intervalos_params_cc;

string tipo_saida="int"; // iniciado em int por padrão

int ultima_pagina, cont_pags, cabecalhos;
string tipo_param;

typedef struct {
	GtkWidget *widget;
	gint index;
	const gchar *title;
	GtkAssistantPageType type;
	gboolean complete;
} PageInfo;

GtkWidget *aux_button0, *aux_button1, *aux_button2, *aux_button3, *aux_button4, *aux_button5;
GtkWidget *assistant, *btn_voltar, *btn_proximo;
GtkWidget *framepag0, *framepag1;

int linha;
gint marg_col_esq=0, marg_col_dir=170;

// Fonte da label
PangoFontDescription *myLabelFontArial32 = pango_font_description_from_string("Arial 32 Bold");
PangoFontDescription *myLabelFontArial14 = pango_font_description_from_string("Arial 14 Bold");
PangoFontDescription *myLabelFontArial10 = pango_font_description_from_string("Arial 10 italic");
PangoFontDescription *myLabelFontArial8 = pango_font_description_from_string("Arial 8");
PangoFontDescription *mylabelfont = pango_font_description_from_string("Arial 14");
PangoFontDescription *comentario = pango_font_description_from_string("Arial 9");
PangoFontDescription *comentario2 = pango_font_description_from_string("Arial 7");

// variaveis auxiliares lixos
int marcouOpcao = 0;

int strToInt (const char str[]) {
    int i, intValue, result, posResult = 0;
    int negative;
     
    if (str[0] == '-') {
        for (i = 1; str[i] >= '0' && str[i] <= '9'; ++i) {   
            intValue = str[i] - '0';
            posResult = posResult * 10 + intValue;
            result = posResult * (-1);
        }
    } else {
        for (i = 0; str[i] >= '0' && str[i] <= '9'; ++i) {   
            intValue = str[i] - '0';
            result = result * 10 + intValue;
        }
    }
    return result;
}

static void entry_page1_changed (GtkEditable *entry, GtkAssistant *assistant) {
	
	bool todosPreenchidos, entradaCorreta;
	string t1 = gtk_entry_get_text (GTK_ENTRY (entry_nome_classe));
	string t2 = gtk_entry_get_text (GTK_ENTRY (entry_nome_metodo));
	string t3 = gtk_entry_get_text (GTK_ENTRY (entry_num_conj_testes));
	string t4 = gtk_entry_get_text (GTK_ENTRY (entry_nome_pacote));
	string t5 = gtk_entry_get_text (GTK_ENTRY (entry_params));
    
    nome_classe = t1;
	nome_pacote = t4;
    nome_metodo = t2;
    parametros = t5;
  	if(t3.length() > 0 && ehNumeroInteiro(t3)) {
	  	numero_conj_testes = atoi( t3.c_str() );
	 	entradaCorreta = true;
	} else
	  	entradaCorreta = false;
	  	
	todosPreenchidos = t1.length()>0 && t2.length()>0 && t3.length()>0 && t5.length()>0;
		
    gint num = gtk_assistant_get_current_page (assistant);
    GtkWidget *page = gtk_assistant_get_nth_page (assistant, num);
    gtk_assistant_set_page_complete (assistant, page, (entradaCorreta && todosPreenchidos));
}

static void button_cancel (GtkWidget *window) {

	GtkWidget *dialog;
	dialog = gtk_message_dialog_new_with_markup(
		GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_OTHER, GTK_BUTTONS_YES_NO,
		"\n<b>Deseja voltar para o menu principal?</b>"
	);
	//gtk_window_set_title(GTK_WINDOW(dialog), "Question");
	gint answer = gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy (dialog);

	if (answer == GTK_RESPONSE_YES) {
		// descarta o que foi armazenado nas especificações dos intervalos
		nome_conj_testes.clear();
		nro_testes_conj.clear();
		saidas_esperadas.clear();
		intervalos_params.clear();
		gtk_widget_destroy (GTK_WIDGET (window));
	    gtk_widget_show (window_root);
	}
}

static void button_cancel2 (GtkWidget *window) {
    gtk_widget_destroy (GTK_WIDGET (window));
    gtk_widget_show (window_root);
}

static void toggle_reverse (GtkToggleButton *button) {
	cc_bool = gtk_toggle_button_get_active (button);
	//cout << "cc_bool = " << cc_bool << endl;
}

static void button_next_cabecalho (GtkWidget *window) {
	bool preencheuDadosIntervalo; preencheuDadosIntervalo = false;
	cont_pags++;
	//cout << "cont_pags = " << cont_pags << endl;
	string t1 = gtk_entry_get_text (GTK_ENTRY (entry_nome_conj_testes));
	string t2 = gtk_entry_get_text (GTK_ENTRY (entry_nro_testes));
	
	if(t1 != "" && t2 != "") {
	
		
		if (find(nome_conj_testes.begin(), nome_conj_testes.end(), t1) != nome_conj_testes.end())
			nomes_conj_repetidos = true;
		
		//cout << "t1: " << t1 << endl;
		//cout << "t2: " << t2 << endl;
		
		if( tipo_saida == "boolean" ) {
			string t3; char ch[10];
			gchar *tipo = gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(combobox_boolean));
			sprintf(ch, "%s", tipo);
			t3 = ch;
			saidas_esperadas.push_back( t3 );
			preencheuDadosIntervalo = true;
			g_free (tipo);
		} else if( tipo_saida=="char" ) {// saida esperada para char
			string t3 = gtk_entry_get_text (GTK_ENTRY (entry_aux1));
			if(t3!="") {
				preencheuDadosIntervalo = true;
				saidas_esperadas.push_back( t3 );
			}
		} else if( tipo_saida=="String" ) {// saida esperada para String
			string t3 = gtk_entry_get_text (GTK_ENTRY (entry_aux1));
			string t4 = gtk_entry_get_text (GTK_ENTRY (entry_aux2));
			if(t3!="" && t4!="" ) {
				preencheuDadosIntervalo = true;
				saidas_esperadas.push_back( t3+"&"+t4 );
			}
		} else {// saida esperada para int/float/double
			string t3 = gtk_entry_get_text (GTK_ENTRY (entry_aux1));
			string t4 = gtk_entry_get_text (GTK_ENTRY (entry_aux2));
			string t5 = gtk_entry_get_text (GTK_ENTRY (entry_aux3));
			if((t3!="" && t4!="") || t5!="") {
				preencheuDadosIntervalo = true;
				if(t5=="")
					saidas_esperadas.push_back( t3+"&"+t4 );
				else if(t3=="" && t4=="")
					saidas_esperadas.push_back( t5 );
				else
					saidas_esperadas.push_back( t3+"&"+t4+"&"+t5 );
			}
		}

		if( preencheuDadosIntervalo ) {
			nome_conj_testes.push_back( t1 );
			nro_testes_conj.push_back( t2 );
			gtk_widget_destroy (GTK_WIDGET (window));
			if( cont_pags % (qtd_params+1) == 0 )
				cabecalho_especificao(window);
			else
				corpo_especificao(window);
		} else {
			cont_pags--;
			GtkWidget *popup = gtk_message_dialog_new_with_markup(
			GTK_WINDOW(window), GTK_DIALOG_MODAL, GTK_MESSAGE_OTHER, GTK_BUTTONS_OK,
				"\n<b>Favor preencher os dados necessários.</b>");
			gtk_dialog_run(GTK_DIALOG(popup));
			gtk_widget_destroy(popup);
		}
		
	} else {
		cont_pags--;
		GtkWidget *popup = gtk_message_dialog_new_with_markup(
		GTK_WINDOW(window), GTK_DIALOG_MODAL, GTK_MESSAGE_OTHER, GTK_BUTTONS_OK,
			"\n<b>Favor preencher os dados necessários.</b>");	
		gtk_dialog_run(GTK_DIALOG(popup));
		gtk_widget_destroy(popup);
	}
}

static void button_next_corpo (GtkWidget *window) {

	bool preencheuDadosIntervalo; preencheuDadosIntervalo = false;
	cont_pags++;

	if( tipo_param == "boolean" ) {
		string t3; char ch[10];
		gchar *tipo = gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(combobox_boolean));
		sprintf(ch, "%s", tipo);
		t3 = ch;
		intervalos_params.push_back( t3 );
		preencheuDadosIntervalo = true;
		g_free (tipo);
	} else if( tipo_param=="char" ) {// intervalo param se for char
	    string t1 = gtk_entry_get_text (GTK_ENTRY (entry_aux1));
		if(t1!="") {
			preencheuDadosIntervalo = true;
			intervalos_params.push_back( t1 );
		}
	} else if( tipo_param=="String" ) {// intervalo param se for String
	    string t1 = gtk_entry_get_text (GTK_ENTRY (entry_aux1));
		string t2 = gtk_entry_get_text (GTK_ENTRY (entry_aux2));
		if(t1!="" && t2!="" ) {
			preencheuDadosIntervalo = true;
			intervalos_params.push_back( t1+"&"+t2 );
		}
	} else {// intervalo param se for int/float/double
	    string t1 = gtk_entry_get_text (GTK_ENTRY (entry_aux1));
		string t2 = gtk_entry_get_text (GTK_ENTRY (entry_aux2));
		string t3 = gtk_entry_get_text (GTK_ENTRY (entry_aux3));
		if((t1!="" && t2!="") || t3!="") {
			preencheuDadosIntervalo = true;
			if(t3 == "")
				intervalos_params.push_back( t1+"&"+t2 );
			else
				intervalos_params.push_back( t1+"&"+t2+"&"+t3 );
		}
	}
	//cout << "cont_pags=" << cont_pags << " / " << "ultima_pagina=" << ultima_pagina << endl;
	//cout << "preencheuDadosIntervalo=" << preencheuDadosIntervalo << endl;
	if(cont_pags == ultima_pagina && preencheuDadosIntervalo) {
		gtk_widget_destroy (GTK_WIDGET (window));
		gtk_widget_show (window_root);
		gtk_widget_set_sensitive (aux_button3, FALSE);
		gtk_widget_set_sensitive (aux_button4, TRUE);
		gtk_button_set_label(GTK_BUTTON(aux_button3), "Conjuntos de testes especificados ✓");
		gtk_button_set_label(GTK_BUTTON(aux_button4), "GERAR TESTES");
		
		cout << "VALORES OBTIDOS ASSISTENTE 2:" << endl;
		cout << "Nomes conjuntos de testes: ";
		for(int z=0; z<nome_conj_testes.size(); z++) {
			if(z==nome_conj_testes.size()-1) cout << nome_conj_testes[z];
			else cout << nome_conj_testes[z] << " ";
		} printf("\n");
		cout << "Números de testes dos conjuntos: ";
		for(int z=0; z<nro_testes_conj.size(); z++) {
			if(z==nro_testes_conj.size()-1) cout << nro_testes_conj[z];
			else cout << nro_testes_conj[z] << " ";
		} printf("\n");
		cout << "Saídas esperadas: " << endl;
		for(int z=0; z<saidas_esperadas.size(); z++)
			cout << z << ": " << saidas_esperadas[z] << endl;
		cout << "Intervalos parâmetros: " << endl;
		for(int z=0; z<intervalos_params.size(); z++)
			cout << z << ": " << intervalos_params[z] << endl;
		if( cc_bool )
			cout << "Caso contrario foi marcado." << endl;
		printf("\n");
	} else if( preencheuDadosIntervalo ) {
		gtk_widget_destroy (GTK_WIDGET (window));
		if( cont_pags % (qtd_params+1) == 0 )
			cabecalho_especificao(window);
		else
			corpo_especificao(window);
	} else {
		cont_pags--;
		GtkWidget *popup = gtk_message_dialog_new_with_markup(
		GTK_WINDOW(window), GTK_DIALOG_MODAL, GTK_MESSAGE_OTHER, GTK_BUTTONS_OK,
			"\n<b>Favor preencher os dados necessários.</b>");
		gtk_dialog_run(GTK_DIALOG(popup));
		gtk_widget_destroy(popup);
	}
	
    
}

// Ao cancelar a janela, deleta memória e limpa a estrutura do assistente
static void assistant_cancel (GtkAssistant *assistant, gpointer data) {
    gtk_widget_destroy (GTK_WIDGET (assistant));
}

// Encerrar assistente
static void assistant_close (GtkAssistant *assistant, gpointer data) {
    //cout << "Testes gerados com sucesso!" << endl;
    gtk_widget_destroy (GTK_WIDGET (assistant));
}

static void constroi_pagina0() {
	linha=1;

	GtkWidget *label_bg;
	
	label_pagina = gtk_label_new("Nome da Classe:");
    gtk_widget_modify_font (label_pagina, myLabelFontArial14);
    gtk_fixed_put(GTK_FIXED(framepag0), label_pagina, marg_col_esq, 25+(linha-1)*50);
	   
// 1a linha ---
    // Label
    label_pagina = gtk_label_new("Nome da Classe:");
    gtk_widget_modify_font (label_pagina, myLabelFontArial14);
    gtk_fixed_put(GTK_FIXED(framepag0), label_pagina, marg_col_esq, 25+(linha-1)*50);
    // TextBox - Entrada Nome da classe
    entry_nome_classe = gtk_entry_new();
    gtk_widget_modify_font (entry_nome_classe, myLabelFontArial10);
    gtk_entry_set_width_chars((GtkEntry *)entry_nome_classe, 35); //tamanho
    gtk_entry_set_max_length (GTK_ENTRY (entry_nome_classe), 40); //nro_caracteres
    gtk_fixed_put(GTK_FIXED(framepag0), entry_nome_classe, marg_col_dir, 20+(linha-1)*50);
	gtk_entry_set_placeholder_text (GTK_ENTRY (entry_nome_classe), "*Obrigatório");

    linha++;
// 2a linha ---
    // Label - Entrada Nome do pacote
    label_pagina = gtk_label_new("Nome do pacote:");
    gtk_widget_modify_font (label_pagina, myLabelFontArial14);
    gtk_fixed_put(GTK_FIXED(framepag0), label_pagina, marg_col_esq, 25+(linha-1)*50);
    // TextBox - Entrada Nome do pacote
    entry_nome_pacote = gtk_entry_new();
    gtk_widget_modify_font (entry_nome_pacote, myLabelFontArial10);
    gtk_entry_set_width_chars((GtkEntry *)entry_nome_pacote, 35); //tamanho
    gtk_entry_set_max_length (GTK_ENTRY (entry_nome_pacote), 40); //nro_caracteres
    gtk_fixed_put(GTK_FIXED(framepag0), entry_nome_pacote, marg_col_dir, 20+(linha-1)*50);
	gtk_entry_set_placeholder_text (GTK_ENTRY (entry_nome_pacote), "*Opcional");

    linha++;
// 3a linha ---
    // Label - Entrada nome do método
    label_pagina = gtk_label_new("Nome do Método:");
    gtk_widget_modify_font (label_pagina, myLabelFontArial14);
    gtk_fixed_put(GTK_FIXED(framepag0), label_pagina, marg_col_esq, 25+(linha-1)*50);
    // TextBox - Entrada Nome do método
    entry_nome_metodo = gtk_entry_new();
    gtk_widget_modify_font (entry_nome_metodo, myLabelFontArial10);
    gtk_entry_set_width_chars((GtkEntry *)entry_nome_metodo, 35); //tamanho
    gtk_entry_set_max_length (GTK_ENTRY (entry_nome_metodo), 40); //nro_caracteres
    gtk_fixed_put(GTK_FIXED(framepag0), entry_nome_metodo, marg_col_dir, 20+(linha-1)*50);
   	gtk_entry_set_placeholder_text (GTK_ENTRY (entry_nome_metodo), "*Obrigatório");

    linha++;
// 4a linha ---
    // Label - Entrada Nome do método
    label_pagina = gtk_label_new("Parâmetro(s):");
    gtk_widget_modify_font (label_pagina, myLabelFontArial14);
    gtk_fixed_put(GTK_FIXED(framepag0), label_pagina, marg_col_esq, 25+(linha-1)*50);
       // Label comentario
    label_pagina = gtk_label_new("(Separados por vírgula)");
    gtk_widget_modify_font (label_pagina, comentario);
    gtk_fixed_put(GTK_FIXED(framepag0), label_pagina, marg_col_esq, 25+(linha-1)*50+20);
    // TextBox - Entrada Nome do método
    entry_params = gtk_entry_new();
    gtk_widget_modify_font (entry_params, myLabelFontArial10);
    gtk_entry_set_width_chars((GtkEntry *)entry_params, 35); //tamanho
    gtk_entry_set_max_length (GTK_ENTRY (entry_params), 60); //nro_caracteres
    gtk_fixed_put(GTK_FIXED(framepag0), entry_params, marg_col_dir, 20+(linha-1)*50);
    gtk_entry_set_placeholder_text (GTK_ENTRY (entry_params), "ex: param1, param2, param3");

    linha++;
// 5a linha ---
    // Label - Entrada Tipo da Saída
    label_pagina = gtk_label_new("Tipo da Saída:");
    gtk_widget_modify_font (label_pagina, myLabelFontArial14);
    gtk_fixed_put(GTK_FIXED(framepag0), label_pagina, marg_col_esq, 25+(linha-1)*50);
    
	/* Create the combo box and append your string values to it. */
	tipo_pag1_combo_box = gtk_combo_box_text_new ();
	const char *distros[] = {"int", "char", "String", "float", "double", "boolean"};
	/* G_N_ELEMENTS is a macro which determines the number of elements in an array.*/ 
	for (guint i = 0; i < G_N_ELEMENTS (distros); i++)
		gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (tipo_pag1_combo_box), distros[i]);

	gtk_combo_box_set_active (GTK_COMBO_BOX (tipo_pag1_combo_box), 0);
	gtk_fixed_put(GTK_FIXED(framepag0), tipo_pag1_combo_box, marg_col_dir, 20+(linha-1)*50);
	
    linha++;
    // Label - Entrada intervalo saída
    label_pagina = gtk_label_new("Número de conjuntos de teste:");
    gtk_widget_modify_font (label_pagina, myLabelFontArial14);
    gtk_fixed_put(GTK_FIXED(framepag0), label_pagina, marg_col_esq, 25+(linha-1)*50);
	// TextBox - Entrada Conjunto de testes
	entry_num_conj_testes = gtk_entry_new();
	gtk_widget_modify_font (entry_num_conj_testes, myLabelFontArial10);
	gtk_entry_set_width_chars((GtkEntry *)entry_num_conj_testes, 5); // tamanho
	gtk_entry_set_max_length (GTK_ENTRY (entry_num_conj_testes), 2); //nro_caracteres
	gtk_fixed_put(GTK_FIXED(framepag0), entry_num_conj_testes, marg_col_dir+140, 20+(linha-1)*50);  
}

static void on_changed_combobox (GtkComboBox *widget) {

	GtkComboBox *combo_box = widget;
	gchar *tipo = gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(combo_box));

	char ch[10];
	sprintf(ch, "%s", tipo);
	tipo_saida = ch;

	g_free (tipo);
}

bool verificaTiposPreenchidos() {
	for(int t=0; t<params.size(); t++)
		if( tipos[t]=="" )
			return false;
			
	return true;
}

static void on_changed_combobox_tipos (GtkComboBox *widget, GtkAssistant *assistant) {

	marcouOpcao++;
	GtkComboBox *combo_box = widget;

	for(int t=0; t<params.size(); t++) {
		gchar *tipo = gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(combo_box_tipo[t]));		
		char ch[10];
		sprintf(ch, "%s", tipo);
		tipos[t] = ch;
		g_free (tipo);
	}

	if( verificaTiposPreenchidos() ) {
		gint num = gtk_assistant_get_current_page (assistant);
		GtkWidget *page = gtk_assistant_get_nth_page (assistant, num);
	    gtk_assistant_set_page_complete (assistant, page, true);
	}
}

static bool ehNumeroInteiroDiferenteDeZero (string entrada) {
	if( ehNumeroInteiro(entrada) ) {
		// verifica se só contém zeros
		for(int i=0; i<entrada.length(); i++)
			if( entrada[i] != 48 ) // ASCII: 48 = 0
				return true;
	} else
		return false;
}

static bool ehNumeroInteiro (string entrada) {
	for(int i=0; i<entrada.length(); i++)
		if(entrada[i] < 48 || entrada[i] > 57)
			if( (entrada[i] == 45 && i>0) || entrada[i] != 45 ) // ASCII: 44 = , / 45 = - / 46 = . 
				return false;

	return true;
}

static bool entradaTamanhoString (string entrada) {

	vector <string> limites; bool varia;
	varia = false;

	for(int i=0; i<entrada.length(); i++) {
		if(entrada[i] < 48 || entrada[i] > 57) {
			if(!(entrada[i] == 126)) // ASCII: 126 = ~
				return false;
			else if((entrada[i] == 126))
				varia = true;
		}
	}

	if(entrada[0]==126 || entrada[entrada.length()-1]==126 )
		return false;
		
	if ( varia ) {
		entrada.erase(remove(entrada.begin(), entrada.end(), ' '), entrada.end());
		limites = split(entrada, '~');
		
		if(limites[1] > limites[0])
			return true;
		else
			return false;
	}
	return true;
}

typedef struct {
	GtkAssistant *assistant;
	GtkWidget    *page;
} PageData;

static void criar_assistente_info_gerais (GtkWidget *widget) {
	
	gtk_widget_hide (window_root);
	static GtkWidget *assistant = NULL;
	PageInfo page[ 2 ] = {
		{ NULL, -1, "Informações do método", GTK_ASSISTANT_PAGE_CONTENT,  FALSE},
		{ NULL, -1, "Confirmação", 			 GTK_ASSISTANT_PAGE_CONFIRM,  TRUE },
	};
    
	assistant = gtk_assistant_new ();
	gtk_widget_set_size_request (assistant, 850, 650);
	gtk_window_set_title (GTK_WINDOW (assistant), "Gerador de casos de teste automatizado para JUnit");
	gtk_window_set_position(GTK_WINDOW(assistant), GTK_WIN_POS_CENTER);
	g_signal_connect (G_OBJECT (assistant), "destroy", G_CALLBACK (gtk_main_quit), NULL);

	page[0].widget = gtk_hbox_new (FALSE, 5);
	page[1].widget = gtk_label_new ("Informações gerais definidas.");

	// Frames and widgets para cada página
	framepag0 = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(page[0].widget), framepag0);

	constroi_pagina0();
	
	/* Add five pages to the GtkAssistant dialog. */
	for (guint i = 0; i < 2; i++) {
		page[i].index = gtk_assistant_append_page (GTK_ASSISTANT (assistant), page[i].widget);
		gtk_assistant_set_page_title (GTK_ASSISTANT (assistant), page[i].widget, page[i].title);
		gtk_assistant_set_page_type (GTK_ASSISTANT (assistant), page[i].widget, page[i].type);

		// Set the introduction and conclusion pages as complete so they can be incremented or closed.
		gtk_assistant_set_page_complete (GTK_ASSISTANT (assistant), page[i].widget, page[i].complete);
	}

	// verifica se pode passar para proxima etapa da pagina 1 ao preencher os campos
	g_signal_connect (tipo_pag1_combo_box, "changed", G_CALLBACK (on_changed_combobox),NULL);
	g_signal_connect (G_OBJECT (entry_nome_classe), "changed", G_CALLBACK (entry_page1_changed), (gpointer) assistant);
	g_signal_connect (G_OBJECT (entry_nome_metodo), "changed", G_CALLBACK (entry_page1_changed), (gpointer) assistant);
	g_signal_connect (G_OBJECT (entry_nome_pacote), "changed", G_CALLBACK (entry_page1_changed), (gpointer) assistant);
	g_signal_connect (G_OBJECT (entry_params), "changed", G_CALLBACK (entry_page1_changed), (gpointer) assistant);
	g_signal_connect (G_OBJECT (entry_num_conj_testes), "changed", G_CALLBACK (entry_page1_changed), (gpointer) assistant);

	g_signal_connect (G_OBJECT (assistant), "cancel", G_CALLBACK (assistant_cancel), NULL);
	g_signal_connect (G_OBJECT (assistant), "close", G_CALLBACK (assistant_close), NULL);
	gtk_widget_show_all (assistant);
    
	gtk_main ();
	
	params.clear();
	// remove espaços em branco da lista de parâmetros
	parametros.erase(remove(parametros.begin(), parametros.end(), ' '), parametros.end());

	// separa parâmetros pelas vírgulas	
	params = split(parametros, ',');
	qtd_params = params.size();
	
	gtk_widget_show (window_root);
	if( numero_conj_testes != 0 ) {
		gtk_widget_set_sensitive (aux_button0, TRUE);
		gtk_widget_set_sensitive (aux_button2, TRUE);
		gtk_button_set_label(GTK_BUTTON(aux_button1), "Redefinir informações do método");
		
		cout << "VALORES OBTIDOS ASSISTENTE 1:" << endl;
		cout << "Classe: " << nome_classe << endl;
		if( nome_pacote.length()==0 )
			cout << "Pacote: ---" << nome_pacote << endl;
		else
			cout << "Pacote: " << nome_pacote << endl;
		cout << "Método: " << nome_metodo << endl;
		cout << params.size() << " parametros: " << parametros << endl;
		cout << "Tipo da saida: " << tipo_saida << endl;
		cout << "Numero de Conjuntos de teste: " << numero_conj_testes << endl;
		printf("\n");
	}
}

static void criar_assistente_tipos (GtkWidget *widget) {
	int linha;
	char msg[80] = {0};
	gtk_widget_hide (window_root);
	static GtkWidget *assistant = NULL;
	
	PageInfo page[ 2 ] = {
		{ NULL, -1, "Tipos dos parâmetros", GTK_ASSISTANT_PAGE_CONTENT,  FALSE},
		{ NULL, -1, "Confirmação", GTK_ASSISTANT_PAGE_CONFIRM,  TRUE },
	};
    
	assistant = gtk_assistant_new ();
	gtk_widget_set_size_request (assistant, 850, 650);
	gtk_window_set_title (GTK_WINDOW (assistant), "Gerador de casos de teste automatizado para JUnit");
	gtk_window_set_position(GTK_WINDOW(assistant), GTK_WIN_POS_CENTER);
	g_signal_connect (G_OBJECT (assistant), "destroy", G_CALLBACK (gtk_main_quit), NULL);

	page[0].widget = gtk_hbox_new (FALSE, 5);
	page[1].widget = gtk_label_new ("Tipos dos parâmetros definidos.");

	// Frames and widgets para cada página
	framepag0 = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(page[0].widget), framepag0);
	
	int cont_tipos; cont_tipos = 0;
	const char *distros[] = {"", "int", "char", "String", "float", "double", "boolean"};
	linha = 0;
	
	for(int z=0; z<qtd_params; z++) {

		g_snprintf(msg, sizeof msg, "Tipo %s:", params[z].c_str());
		label_pagina = gtk_label_new(msg);
		gtk_widget_modify_font (label_pagina, myLabelFontArial14);
		gtk_fixed_put(GTK_FIXED(framepag0), label_pagina, marg_col_esq, 25+linha*50);

		combo_box_tipo[linha] = gtk_combo_box_text_new ();
		for (guint i = 0; i < G_N_ELEMENTS (distros); i++)
			gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo_box_tipo[linha]), distros[i]);

		gtk_combo_box_set_active (GTK_COMBO_BOX (combo_box_tipo[linha]), 0);
		gtk_fixed_put(GTK_FIXED(framepag0), combo_box_tipo[linha], marg_col_esq+160, 20+linha*50);
		linha++;
	}
	
	for (guint i = 0; i < 2; i++) {
		page[i].index = gtk_assistant_append_page (GTK_ASSISTANT (assistant), page[i].widget);
		gtk_assistant_set_page_title (GTK_ASSISTANT (assistant), page[i].widget, page[i].title);
		gtk_assistant_set_page_type (GTK_ASSISTANT (assistant), page[i].widget, page[i].type);
		gtk_assistant_set_page_complete (GTK_ASSISTANT (assistant), page[i].widget, page[i].complete);
	}

	for(int z=0; z<params.size(); z++)
		g_signal_connect (combo_box_tipo[z], "changed", G_CALLBACK (on_changed_combobox_tipos), (gpointer) assistant);

	g_signal_connect (G_OBJECT (assistant), "cancel", G_CALLBACK (assistant_cancel), NULL);
	g_signal_connect (G_OBJECT (assistant), "close", G_CALLBACK (assistant_close), NULL);
	gtk_widget_show_all (assistant);
    
	gtk_main ();
	gtk_widget_show (window_root);
	
	gtk_button_set_label(GTK_BUTTON(aux_button1), "Informações do método definidas ✓");
	gtk_button_set_label(GTK_BUTTON(aux_button2), "Tipos dos parâmetros definidos ✓");
	
	if( verificaTiposPreenchidos() ) {
		gtk_widget_set_sensitive (aux_button1, FALSE);
		gtk_widget_set_sensitive (aux_button2, FALSE);
		gtk_widget_set_sensitive (aux_button3, TRUE);
				
		cout << "TIPOS DOS PARAMS:" << endl;
		for(int t=0; t<params.size(); t++)
			cout << "tipos[" << t << "] = " << tipos[t] << endl;
		printf("\n");
	}
}

static void visualizador_dados_entrada(GtkWidget *p_widget) {
	GtkWidget *window;
	GtkWidget *framepage;

	if( nome_classe == "" ) {
	
		GtkWidget *popup = gtk_message_dialog_new_with_markup(
			GTK_WINDOW(p_widget), GTK_DIALOG_MODAL, GTK_MESSAGE_OTHER, GTK_BUTTONS_OK,
			"\n<b>Dados de entrada ainda não foram preenchidos.</b>");

		gtk_dialog_run(GTK_DIALOG(popup));
		gtk_widget_destroy(popup);
	
	} else {
		char msg[80] = {0}; int linha = 0;
	
		window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		gtk_window_set_title(GTK_WINDOW(window), "Visualizar dados de entrada");
		gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
		gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
		gtk_window_set_default_size(GTK_WINDOW(window), 800, 320);
	
		framepage = gtk_fixed_new();
		gtk_container_add(GTK_CONTAINER(window), framepage);
		
		g_snprintf(msg, sizeof msg, "Classe: %s", nome_classe.c_str());
		label_pagina = gtk_label_new(msg);
		gtk_widget_modify_font (label_pagina, myLabelFontArial10);
		gtk_fixed_put(GTK_FIXED(framepage), label_pagina, 22, 20+linha*50);
		
		linha++;
		if(nome_pacote == "")
			g_snprintf(msg, sizeof msg, "Pacote: -");
		else
			g_snprintf(msg, sizeof msg, "Pacote: %s", nome_pacote.c_str());
		label_pagina = gtk_label_new(msg);
		gtk_widget_modify_font (label_pagina, myLabelFontArial10);
		gtk_fixed_put(GTK_FIXED(framepage), label_pagina, 22, 20+linha*35);
		
		linha++;
		g_snprintf(msg, sizeof msg, "Método: %s", nome_metodo.c_str());
		label_pagina = gtk_label_new(msg);
		gtk_widget_modify_font (label_pagina, myLabelFontArial10);
		gtk_fixed_put(GTK_FIXED(framepage), label_pagina, 22, 20+linha*35);
		
		linha++;
		g_snprintf(msg, sizeof msg, "Parâmetros: %s", parametros.c_str());
		label_pagina = gtk_label_new(msg);
		gtk_widget_modify_font (label_pagina, myLabelFontArial10);
		gtk_fixed_put(GTK_FIXED(framepage), label_pagina, 22, 20+linha*35);
		
		linha++;
		g_snprintf(msg, sizeof msg, "Tipo da saída: %s", tipo_saida.c_str());
		label_pagina = gtk_label_new(msg);
		gtk_widget_modify_font (label_pagina, myLabelFontArial10);
		gtk_fixed_put(GTK_FIXED(framepage), label_pagina, 22, 20+linha*35);

		linha++;
		g_snprintf(msg, sizeof msg, "Número de conjunto(s) de testes: %d", numero_conj_testes);
		label_pagina = gtk_label_new(msg);
		gtk_widget_modify_font (label_pagina, myLabelFontArial10);
		gtk_fixed_put(GTK_FIXED(framepage), label_pagina, 22, 20+linha*35);

		char todostipos[120];
		if( verificaTiposPreenchidos() ) {
			linha++;
			label_pagina = gtk_label_new("Tipos dos parâmetros:");
			gtk_widget_modify_font (label_pagina, myLabelFontArial10);
			gtk_fixed_put(GTK_FIXED(framepage), label_pagina, 22, 20+linha*35);
			strcpy(todostipos,tipos[0].c_str());
			for(int z=1; z<params.size(); z++) {
				strcat(todostipos, ", ");
				strcat(todostipos, tipos[z].c_str());
			}
			strcpy(msg, todostipos); linha++;
			label_pagina = gtk_label_new(msg);
			gtk_widget_modify_font (label_pagina, myLabelFontArial10);
			gtk_fixed_put(GTK_FIXED(framepage), label_pagina, 22, 20+linha*35);
		}

		if( nome_conj_testes.size() == nro_testes_conj.size() && nome_conj_testes.size() == numero_conj_testes) {
			linha=0;
			label_pagina = gtk_label_new("Conjuntos de testes:");
			gtk_widget_modify_font (label_pagina, myLabelFontArial10);
			gtk_fixed_put(GTK_FIXED(framepage), label_pagina, 340, 20+linha*35);	
			for(int z=0; z<numero_conj_testes; z++) {
				linha++;
				g_snprintf(msg, sizeof msg, " - %s terá %s testes", nome_conj_testes[z].c_str(), nro_testes_conj[z].c_str());
				label_pagina = gtk_label_new(msg);
				gtk_widget_modify_font (label_pagina, myLabelFontArial10);
				gtk_fixed_put(GTK_FIXED(framepage), label_pagina, 340, 20+linha*35);
			}
		}

		linha=8;		
		btn_voltar = gtk_button_new_with_label ("Voltar");
		gtk_fixed_put(GTK_FIXED(framepage), btn_voltar, 350, 20+linha*35);
		g_signal_connect_swapped (btn_voltar, "clicked", G_CALLBACK (button_cancel2), window);

		gtk_widget_show_all(window);
	}
}

static void gerar_testes(GtkWidget *p_widget) {
	GtkWidget *window;
	GtkWidget *framepage;
	char filename[50]; bool falhou = false;
	
	inicializa_arq_saida(nome_classe, nome_pacote);
	
	if( nomes_conj_repetidos ) {
		cout << "Conjuntos de teste com nomes repetidos." << endl;
		falhou = true;
	} else {		
		for(int z=0; z<numero_conj_testes; z++) {
			for(int y=0; y<atoi(nro_testes_conj[z].c_str()); y++) {
				if( !gerar(nome_metodo, tipos, saidas_esperadas[z], intervalos_params, tipo_saida, nome_conj_testes[z], y+1, params.size(), z) ) {
					falhou = true; break;
				}
				
				/*if(tipo_saida=="boolean" && cc_bool) {
					cout << "Chegou no caso contrário22." << endl;
					if( !gerar(nome_metodo, tipos, saidas_esperadas_cc[z], intervalos_params_cc, tipo_saida, nome_conj_testes_cc[z], y+1, params.size(), z) ) {
						falhou = true; break;
					}
				}*/
			}
		}
	}
	encerra_arq_saida();
	
	if( falhou ) {
		printf("\nNÃO FOI POSSÍVEL GERAR OS TESTES.\n", nome_classe.c_str());
		
		GtkWidget *popup = gtk_message_dialog_new_with_markup(
			GTK_WINDOW(window_root), GTK_DIALOG_MODAL, GTK_MESSAGE_OTHER, GTK_BUTTONS_OK,
			"\n<b>Não foi possível gerar os testes.</b>");
		gtk_dialog_run(GTK_DIALOG(popup));
		gtk_widget_destroy(popup);
		gtk_widget_destroy(window_root);
	} else {
		
		printf("TESTES GERADOS COM SUCESSO.\n", nome_classe.c_str());
		printf("%sTeste.java\n", nome_classe.c_str());
		
		GtkWidget *popup = gtk_message_dialog_new_with_markup(
			GTK_WINDOW(window_root), GTK_DIALOG_MODAL, GTK_MESSAGE_OTHER, GTK_BUTTONS_OK,
			"\n<b>Testes gerados com sucesso!</b>");
		gtk_dialog_run(GTK_DIALOG(popup));
		gtk_widget_destroy(popup);
		gtk_widget_destroy(window_root);
	}
}

static void definir_conjuntos_testes(GtkWidget *p_widget) {

	// inicializa valores das variaveis
	cont_pags = 0; cabecalhos=0;
	ultima_pagina = qtd_params * numero_conj_testes + numero_conj_testes;
	//cout << "ultima pagina: " << ultima_pagina << endl;
	if( numero_conj_testes == 0 ) {
		GtkWidget *popup = gtk_message_dialog_new_with_markup(
			NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_OTHER, GTK_BUTTONS_OK,
			"\n<b>É necessário preencher as informações do método primeiro.</b>"
		);
		gtk_dialog_run(GTK_DIALOG(popup));
		gtk_widget_destroy(popup);
	} else
   		cabecalho_especificao(p_widget);
}

static void cabecalho_especificao(GtkWidget *p_widget) {

	GtkWidget *window, *framepage;
	int linha = 1; char msg[80] = {0};
	
	gtk_widget_hide (window_root);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Novo conjunto de testes");
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), 720, 550);
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
	gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

	framepage = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(window), framepage);
	
	linha=1; cabecalhos++;
	g_snprintf(msg, sizeof msg, "Conjunto de testes %d de %d", cabecalhos, numero_conj_testes);
	label_pagina = gtk_label_new(msg);
	gtk_widget_modify_font (label_pagina, myLabelFontArial14);
	gtk_fixed_put(GTK_FIXED(framepage), label_pagina, marg_col_esq+15, 25+(linha-1)*50);
	
	linha++;
	label_pagina = gtk_label_new("Nome do conjunto de testes:");
	gtk_widget_modify_font (label_pagina, myLabelFontArial10);
	gtk_fixed_put(GTK_FIXED(framepage), label_pagina, marg_col_esq+15, 25+(linha-1)*50);
	// entrada nome conjunto testes
	entry_nome_conj_testes = gtk_entry_new();
	gtk_widget_modify_font (entry_nome_conj_testes, myLabelFontArial10);
	gtk_entry_set_width_chars((GtkEntry *)entry_nome_conj_testes, 30);
	gtk_entry_set_max_length (GTK_ENTRY (entry_nome_conj_testes), 35);
	gtk_fixed_put(GTK_FIXED(framepage), entry_nome_conj_testes, marg_col_dir+85, 20+(linha-1)*50);
	gtk_entry_set_placeholder_text (GTK_ENTRY (entry_nome_conj_testes), "Sem espaços");
	
	linha++;
	label_pagina = gtk_label_new("Número de testes a serem gerados:");
	gtk_widget_modify_font (label_pagina, myLabelFontArial10);
	gtk_fixed_put(GTK_FIXED(framepage), label_pagina, marg_col_esq+15, 25+(linha-1)*50);
	// entrada numero de testes
	entry_nro_testes = gtk_entry_new();
	gtk_widget_modify_font (entry_nro_testes, myLabelFontArial10);
	gtk_entry_set_width_chars((GtkEntry *)entry_nro_testes, 5);
	gtk_entry_set_max_length (GTK_ENTRY (entry_nro_testes), 4);
	gtk_fixed_put(GTK_FIXED(framepage), entry_nro_testes, marg_col_dir+145, 20+(linha-1)*50);
	
	linha++;
	g_snprintf(msg, sizeof msg, "Saída esperada (%s):", tipo_saida.c_str());
	label_pagina = gtk_label_new(msg);
	gtk_widget_modify_font (label_pagina, myLabelFontArial14);
	gtk_fixed_put(GTK_FIXED(framepage), label_pagina, marg_col_esq+15, 25+(linha-1)*50);

	if( tipo_saida=="boolean" ) {// saida esperada para boolean
	
		const char *distros2[] = {"true", "false"};
		combobox_boolean = gtk_combo_box_text_new ();
		for (guint i = 0; i < G_N_ELEMENTS (distros2); i++)
			gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combobox_boolean), distros2[i]);
		gtk_combo_box_set_active (GTK_COMBO_BOX (combobox_boolean), 0);
		gtk_fixed_put(GTK_FIXED(framepage), combobox_boolean,  marg_col_esq+242, 18+(linha-1)*50);
		
		linha++;
		toggle = gtk_check_button_new_with_label ("Em caso contrário dos intervalos dos parâmetros\nque serão especificados a seguir, a saída esperada\nserá sempre a oposta à definida acima.");
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (toggle), cc_bool);
		g_signal_connect (G_OBJECT (toggle), "toggled", G_CALLBACK (toggle_reverse), NULL);
		gtk_fixed_put(GTK_FIXED(framepage), toggle, marg_col_esq+15, 25+(linha-1)*50);

	} else if( tipo_saida=="char" ) {// saida esperada para char

		linha++;
		label_pagina = gtk_label_new("Incluir:");
		gtk_widget_modify_font (label_pagina, myLabelFontArial10);
		gtk_fixed_put(GTK_FIXED(framepage), label_pagina, marg_col_esq+15, 25+(linha-1)*50);

		// Label comentario
		//label_pagina = gtk_label_new("(Para mais de uma opção, separar por vírgula)");
		//gtk_widget_modify_font (label_pagina, comentario);
		//gtk_fixed_put(GTK_FIXED(framepage), label_pagina, marg_col_esq, 25+(linha-1)*50+30);

		entry_aux1 = gtk_entry_new();
		gtk_widget_modify_font (entry_aux1, myLabelFontArial10);
		gtk_entry_set_width_chars((GtkEntry *)entry_aux1, 30);
		gtk_entry_set_max_length (GTK_ENTRY (entry_aux1), 50);
		gtk_fixed_put(GTK_FIXED(framepage), entry_aux1, marg_col_esq+80, 18+(linha-1)*50);
		gtk_entry_set_placeholder_text (GTK_ENTRY (entry_aux1), "ex: s,n,0,1,-,#,$,%,*");

		/*linha++;
		label_pagina = gtk_label_new("Excluir:");
		gtk_widget_modify_font (label_pagina, myLabelFontArial10);
		gtk_fixed_put(GTK_FIXED(framepage), label_pagina, marg_col_esq+15, 25+(linha-1)*50);

		entry_aux2 = gtk_entry_new();
		gtk_widget_modify_font (entry_aux2, myLabelFontArial10);
		gtk_entry_set_width_chars((GtkEntry *)entry_aux2, 30);
		gtk_entry_set_max_length (GTK_ENTRY (entry_aux2), 50);
		gtk_fixed_put(GTK_FIXED(framepage), entry_aux2, marg_col_esq+80, 18+(linha-1)*50);
		gtk_entry_set_placeholder_text (GTK_ENTRY (entry_aux2), "ex: letras,simbolos ou #,$,%,*");*/

		// Dica: conjuntos pré-definidos
		//label_pagina = gtk_label_new("*Conjuntos pré-definidos:\
		\n   - todos\n   - letras\n   - numeros\n   - alfanumericos\n   - simbolos");
		//gtk_widget_modify_font (label_pagina, myLabelFontArial8);
		//gtk_fixed_put(GTK_FIXED(framepage), label_pagina, marg_col_esq+525, 220);
		
	} else if( tipo_saida=="String" ) {// saida esperada para String

		linha++;
		label_pagina = gtk_label_new("Substrings:");
		gtk_widget_modify_font (label_pagina, myLabelFontArial10);
		gtk_fixed_put(GTK_FIXED(framepage), label_pagina, marg_col_esq+15, 25+(linha-1)*50);

		entry_aux1 = gtk_entry_new();
		gtk_widget_modify_font (entry_aux1, myLabelFontArial10);
		gtk_entry_set_width_chars((GtkEntry *)entry_aux1, 40);
		gtk_entry_set_max_length (GTK_ENTRY (entry_aux1), 60);
		gtk_fixed_put(GTK_FIXED(framepage), entry_aux1, marg_col_dir-45, 5+(linha-1)*50+15);
		gtk_entry_set_placeholder_text (GTK_ENTRY (entry_aux1), "ex: [-][numeros]");

		linha++;
		label_pagina = gtk_label_new("Tamanhos\nSubstrings:");
		gtk_widget_modify_font (label_pagina, myLabelFontArial10);
		gtk_fixed_put(GTK_FIXED(framepage), label_pagina, marg_col_esq+15, 25+(linha-1)*50);

		entry_aux2 = gtk_entry_new();
		gtk_widget_modify_font (entry_aux2, myLabelFontArial10);
		gtk_entry_set_width_chars((GtkEntry *)entry_aux2, 40);
		gtk_entry_set_max_length (GTK_ENTRY (entry_aux2), 60);
		gtk_fixed_put(GTK_FIXED(framepage), entry_aux2, marg_col_dir-45, 5+(linha-1)*50+15);
		gtk_entry_set_placeholder_text (GTK_ENTRY (entry_aux2), "ex: [1][4] ou [1][1~4]");
		
		// Dica: conjuntos pré-definidos
		label_pagina = gtk_label_new("*Conjuntos pré-definidos:\
		\n   - todos\n   - letras\n   - numeros\n   - alfanumericos\n   - simbolos");
		gtk_widget_modify_font (label_pagina, myLabelFontArial8);
		gtk_fixed_put(GTK_FIXED(framepage), label_pagina, marg_col_esq+525, 220);
	
	} else {// saida esperada para int/float/double

		linha++;
		label_pagina = gtk_label_new("De");
		gtk_widget_modify_font (label_pagina, myLabelFontArial10);
		gtk_fixed_put(GTK_FIXED(framepage), label_pagina, marg_col_esq+15, 25+(linha-1)*50);

		entry_aux1 = gtk_entry_new();
		gtk_widget_modify_font (entry_aux1, myLabelFontArial10);
		gtk_entry_set_width_chars((GtkEntry *)entry_aux1, 8);
		gtk_entry_set_max_length (GTK_ENTRY (entry_aux1), 8);
		gtk_fixed_put(GTK_FIXED(framepage), entry_aux1, marg_col_esq+80, 18+(linha-1)*50);

		label_pagina = gtk_label_new("até");
		gtk_widget_modify_font (label_pagina, myLabelFontArial10);
		gtk_fixed_put(GTK_FIXED(framepage), label_pagina, marg_col_esq+195, 25+(linha-1)*50);

		entry_aux2 = gtk_entry_new();
		gtk_widget_modify_font (entry_aux2, myLabelFontArial10);
		gtk_entry_set_width_chars((GtkEntry *)entry_aux2, 8);
		gtk_entry_set_max_length (GTK_ENTRY (entry_aux2), 8);
		gtk_fixed_put(GTK_FIXED(framepage), entry_aux2, marg_col_esq+250, 18+(linha-1)*50);
		
		linha++;
		label_pagina = gtk_label_new("Incluir:");
		gtk_widget_modify_font (label_pagina, myLabelFontArial10);
		gtk_fixed_put(GTK_FIXED(framepage), label_pagina, marg_col_esq+15, 25+(linha-1)*50);

		entry_aux3 = gtk_entry_new();
		gtk_widget_modify_font (entry_aux3, myLabelFontArial10);
		gtk_entry_set_width_chars((GtkEntry *)entry_aux3, 30);
		gtk_entry_set_max_length (GTK_ENTRY (entry_aux3), 50);
		gtk_fixed_put(GTK_FIXED(framepage), entry_aux3, marg_col_esq+80, 18+(linha-1)*50);
		gtk_entry_set_placeholder_text (GTK_ENTRY (entry_aux3), "ex: 10,100,1000");

		/*linha++;
		label_pagina = gtk_label_new("Excluir:");
		gtk_widget_modify_font (label_pagina, myLabelFontArial10);
		gtk_fixed_put(GTK_FIXED(framepage), label_pagina, marg_col_esq+15, 25+(linha-1)*50);

		entry_aux4 = gtk_entry_new();
		gtk_widget_modify_font (entry_aux4, myLabelFontArial10);
		gtk_entry_set_width_chars((GtkEntry *)entry_aux4, 30);
		gtk_entry_set_max_length (GTK_ENTRY (entry_aux4), 50);
		gtk_fixed_put(GTK_FIXED(framepage), entry_aux4, marg_col_esq+80, 18+(linha-1)*50);
		gtk_entry_set_placeholder_text (GTK_ENTRY (entry_aux4), "ex: -1, 0, 1");*/
	}
	
	linha = 10;
	btn_voltar = gtk_button_new_with_label ("Cancelar");
	gtk_fixed_put(GTK_FIXED(framepage), btn_voltar, 270, 25+(linha-1)*48);
	g_signal_connect_swapped (btn_voltar, "clicked", G_CALLBACK (button_cancel), window);

	btn_proximo = gtk_button_new_with_label ("Próximo");
	gtk_fixed_put(GTK_FIXED(framepage), btn_proximo, 390, 25+(linha-1)*48);
	g_signal_connect_swapped (btn_proximo, "clicked", G_CALLBACK (button_next_cabecalho), window);
	
	gtk_widget_show_all (window);
	gtk_main ();
}

static void corpo_especificao(GtkWidget *p_widget) {

	GtkWidget *window, *framepage;
	int linha = 1; char msg[80] = {0};
	
	gtk_widget_hide (window_root);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Intervalo dos parâmetros");
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), 720, 550);
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
	gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

	framepage = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(window), framepage);
	
	linha=1;
		
	g_snprintf(msg, sizeof msg, "Conjunto de teste: %s", nome_conj_testes[ cont_pags/(qtd_params+1) ].c_str());
	label_pagina = gtk_label_new(msg);
	gtk_widget_modify_font (label_pagina, myLabelFontArial14);
	gtk_fixed_put(GTK_FIXED(framepage), label_pagina, marg_col_esq+15, 25+(linha-1)*50);

	linha++;	
	g_snprintf(msg, sizeof msg, "Intervalo de %s (%s):", params[ (cont_pags-1)%(qtd_params+1) ].c_str(), tipos[ (cont_pags-1)%(qtd_params+1) ].c_str());
	label_pagina = gtk_label_new(msg);
	gtk_widget_modify_font (label_pagina, myLabelFontArial14);
	gtk_fixed_put(GTK_FIXED(framepage), label_pagina, marg_col_esq+15, 25+(linha-1)*50);
	
	//aq1
	tipo_param = tipos[ (cont_pags-1)%(qtd_params+1) ];
	
	if( tipo_param=="boolean" ) {// intervalo do param se for boolean
	
		const char *distros2[] = {"true", "false", "random"};
		combobox_boolean = gtk_combo_box_text_new ();
		for (guint i = 0; i < G_N_ELEMENTS (distros2); i++)
			gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combobox_boolean), distros2[i]);
		gtk_combo_box_set_active (GTK_COMBO_BOX (combobox_boolean), 0);
		gtk_fixed_put(GTK_FIXED(framepage), combobox_boolean,  marg_col_esq+242, 18+(linha-1)*50);
		

	} else if( tipo_param=="char" ) {// intervalo do param se for char

		linha++;
		label_pagina = gtk_label_new("Incluir:");
		gtk_widget_modify_font (label_pagina, myLabelFontArial10);
		gtk_fixed_put(GTK_FIXED(framepage), label_pagina, marg_col_esq+15, 25+(linha-1)*50);

		entry_aux1 = gtk_entry_new();
		gtk_widget_modify_font (entry_aux1, myLabelFontArial10);
		gtk_entry_set_width_chars((GtkEntry *)entry_aux1, 30);
		gtk_entry_set_max_length (GTK_ENTRY (entry_aux1), 50);
		gtk_fixed_put(GTK_FIXED(framepage), entry_aux1, marg_col_esq+80, 18+(linha-1)*50);
		gtk_entry_set_placeholder_text (GTK_ENTRY (entry_aux1), "ex: letras,simbolos ou #,$,%,*");

		/*linha++;
		label_pagina = gtk_label_new("Excluir:");
		gtk_widget_modify_font (label_pagina, myLabelFontArial10);
		gtk_fixed_put(GTK_FIXED(framepage), label_pagina, marg_col_esq+15, 25+(linha-1)*50);

		entry_aux2 = gtk_entry_new();
		gtk_widget_modify_font (entry_aux2, myLabelFontArial10);
		gtk_entry_set_width_chars((GtkEntry *)entry_aux2, 30);
		gtk_entry_set_max_length (GTK_ENTRY (entry_aux2), 50);
		gtk_fixed_put(GTK_FIXED(framepage), entry_aux2, marg_col_esq+80, 18+(linha-1)*50);
		gtk_entry_set_placeholder_text (GTK_ENTRY (entry_aux2), "ex: letras,simbolos ou #,$,%,*");*/

		// Dica: conjuntos pré-definidos
		label_pagina = gtk_label_new("*Conjuntos pré-definidos:\
		\n   - todos\n   - letras\n   - numeros\n   - alfanumericos\n   - simbolos\n   - espaco");

		gtk_widget_modify_font (label_pagina, myLabelFontArial8);
		gtk_fixed_put(GTK_FIXED(framepage), label_pagina, marg_col_esq+460, 25+(linha-2)*50);
		
	} else if( tipo_param=="String" ) {// intervalo do param se for String

		linha++;
		label_pagina = gtk_label_new("Substring(s):");
		gtk_widget_modify_font (label_pagina, myLabelFontArial10);
		gtk_fixed_put(GTK_FIXED(framepage), label_pagina, marg_col_esq+15, 25+(linha-1)*50);

		entry_aux1 = gtk_entry_new();
		gtk_widget_modify_font (entry_aux1, myLabelFontArial10);
		gtk_entry_set_width_chars((GtkEntry *)entry_aux1, 40);
		gtk_entry_set_max_length (GTK_ENTRY (entry_aux1), 60);
		gtk_fixed_put(GTK_FIXED(framepage), entry_aux1, marg_col_dir-45, 5+(linha-1)*50+15);
		gtk_entry_set_placeholder_text (GTK_ENTRY (entry_aux1), "ex: [-][numeros]");

		linha++;
		label_pagina = gtk_label_new("Tamanho(s)\nSubstring(s):");
		gtk_widget_modify_font (label_pagina, myLabelFontArial10);
		gtk_fixed_put(GTK_FIXED(framepage), label_pagina, marg_col_esq+15, 25+(linha-1)*50);

		entry_aux2 = gtk_entry_new();
		gtk_widget_modify_font (entry_aux2, myLabelFontArial10);
		gtk_entry_set_width_chars((GtkEntry *)entry_aux2, 40);
		gtk_entry_set_max_length (GTK_ENTRY (entry_aux2), 60);
		gtk_fixed_put(GTK_FIXED(framepage), entry_aux2, marg_col_dir-45, 5+(linha-1)*50+15);
		gtk_entry_set_placeholder_text (GTK_ENTRY (entry_aux2), "ex: [1][4] ou [1][1~4]");
		
		// Dica: conjuntos pré-definidos
		label_pagina = gtk_label_new("*Conjuntos pré-definidos:\
		\n   - todos\n   - letras\n   - numeros\n   - alfanumericos\n   - simbolos");
		gtk_widget_modify_font (label_pagina, myLabelFontArial8);
		gtk_fixed_put(GTK_FIXED(framepage), label_pagina, marg_col_esq+560, 25+(linha-2)*50);
	
	} else {// intervalo do param se for int/float/double

		linha++;
		label_pagina = gtk_label_new("De");
		gtk_widget_modify_font (label_pagina, myLabelFontArial10);
		gtk_fixed_put(GTK_FIXED(framepage), label_pagina, marg_col_esq+15, 25+(linha-1)*50);

		entry_aux1 = gtk_entry_new();
		gtk_widget_modify_font (entry_aux1, myLabelFontArial10);
		gtk_entry_set_width_chars((GtkEntry *)entry_aux1, 8);
		gtk_entry_set_max_length (GTK_ENTRY (entry_aux1), 8);
		gtk_fixed_put(GTK_FIXED(framepage), entry_aux1, marg_col_esq+80, 18+(linha-1)*50);

		label_pagina = gtk_label_new("até");
		gtk_widget_modify_font (label_pagina, myLabelFontArial10);
		gtk_fixed_put(GTK_FIXED(framepage), label_pagina, marg_col_esq+195, 25+(linha-1)*50);

		entry_aux2 = gtk_entry_new();
		gtk_widget_modify_font (entry_aux2, myLabelFontArial10);
		gtk_entry_set_width_chars((GtkEntry *)entry_aux2, 8);
		gtk_entry_set_max_length (GTK_ENTRY (entry_aux2), 8);
		gtk_fixed_put(GTK_FIXED(framepage), entry_aux2, marg_col_esq+250, 18+(linha-1)*50);
		
		linha++;
		label_pagina = gtk_label_new("Incluir:");
		gtk_widget_modify_font (label_pagina, myLabelFontArial10);
		gtk_fixed_put(GTK_FIXED(framepage), label_pagina, marg_col_esq+15, 25+(linha-1)*50);

		entry_aux3 = gtk_entry_new();
		gtk_widget_modify_font (entry_aux3, myLabelFontArial10);
		gtk_entry_set_width_chars((GtkEntry *)entry_aux3, 30);
		gtk_entry_set_max_length (GTK_ENTRY (entry_aux3), 50);
		gtk_fixed_put(GTK_FIXED(framepage), entry_aux3, marg_col_esq+80, 18+(linha-1)*50);
		gtk_entry_set_placeholder_text (GTK_ENTRY (entry_aux3), "ex: 10,100,1000");

		/*linha++;
		label_pagina = gtk_label_new("Excluir:");
		gtk_widget_modify_font (label_pagina, myLabelFontArial10);
		gtk_fixed_put(GTK_FIXED(framepage), label_pagina, marg_col_esq+15, 25+(linha-1)*50);

		entry_aux4 = gtk_entry_new();
		gtk_widget_modify_font (entry_aux4, myLabelFontArial10);
		gtk_entry_set_width_chars((GtkEntry *)entry_aux4, 30);
		gtk_entry_set_max_length (GTK_ENTRY (entry_aux4), 50);
		gtk_fixed_put(GTK_FIXED(framepage), entry_aux4, marg_col_esq+80, 18+(linha-1)*50);
		gtk_entry_set_placeholder_text (GTK_ENTRY (entry_aux4), "ex: -1, 0, 1");*/
	}
	
	linha++;
	btn_voltar = gtk_button_new_with_label ("Cancelar");
	gtk_fixed_put(GTK_FIXED(framepage), btn_voltar, 290, 25+(linha-1)*48);
	g_signal_connect_swapped (btn_voltar, "clicked", G_CALLBACK (button_cancel), window);

	btn_proximo = gtk_button_new_with_label ("Próximo");
	gtk_fixed_put(GTK_FIXED(framepage), btn_proximo, 370, 25+(linha-1)*48);
	g_signal_connect_swapped (btn_proximo, "clicked", G_CALLBACK (button_next_corpo), window);
	
	gtk_widget_show_all (window);
	gtk_main ();
}

void mostrar_sobre(GtkWidget *widget) {
	GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file("assets/logo.png", NULL);
	GtkWidget *dialog = gtk_about_dialog_new();
	//gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(dialog), "AutomTest 1.0 © 2019");
	gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), "1.0"); 
	gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog),"© 2019 Daniel David Fernandes");
	gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), "Gerador de casos de testes para JUnit baseado em requisitos funcionais.");
	gtk_about_dialog_set_website_label(GTK_ABOUT_DIALOG(dialog), "LinkedIn");
	gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog), "http://www.linkedin.com/in/daniel-david-431a4216a");
	gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(dialog), pixbuf);
	gtk_about_dialog_set_license(GTK_ABOUT_DIALOG(dialog), "info adicional");
	gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog), "AutomTest");
	gtk_window_set_transient_for (GTK_WINDOW(dialog), GTK_WINDOW(window_root));
	g_object_unref(pixbuf), pixbuf = NULL;
	gtk_dialog_run(GTK_DIALOG (dialog));
	gtk_widget_destroy(dialog);
}

