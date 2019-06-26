// g++ -w main.cpp `pkg-config --libs --cflags gtk+-3.0`
#include "interface.hpp"

int main (int argc, gchar *argv[]) {

	GtkWidget *box, *button;
	gint i;
	gtk_init (&argc, &argv);

	if (g_getenv ("RTL"))
		gtk_widget_set_default_direction (GTK_TEXT_DIR_RTL);

	window_root = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request (window_root, 720, 550);
	gtk_window_set_title (GTK_WINDOW (window_root), "AutomTest - Gerador de casos de teste automatizado para JUnit");
	gtk_window_set_position(GTK_WINDOW(window_root), GTK_WIN_POS_CENTER);

	g_signal_connect (G_OBJECT (window_root), "destroy", G_CALLBACK (gtk_main_quit), NULL);
	g_signal_connect (G_OBJECT (window_root), "delete-event", G_CALLBACK (gtk_false), NULL);

	box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 6);
	gtk_container_add (GTK_CONTAINER (window_root), box);

	for (i = 0; i < G_N_ELEMENTS (buttons); i++) {
		button = gtk_button_new_with_label (buttons[i].text);

		if (buttons[i].func)
			g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (buttons[i].func), NULL);
		
		if(i==0) {
			aux_button0 = button;
			gtk_widget_set_sensitive (button, FALSE);
		} else if(i==1) {
			aux_button1 = button;
		} else if(i==2) {
			aux_button2 = button;
			gtk_widget_set_sensitive (button, FALSE);
		} else if(i==3) {
			aux_button3 = button;
			gtk_widget_set_sensitive (button, FALSE);
		} else if(i==4) {
			aux_button4 = button;
			gtk_widget_set_sensitive (button, FALSE);
		} else if(i==5) {
			aux_button5 = button;
		}
		
		/*
		0 { "Visualizar dados de entrada", visualizador_dados_entrada },
		1 { "Definir informações do método", criar_assistente_info_gerais },
		2 { "Definir tipos dos parâmetros", criar_assistente_tipos },
		3 { "Especificar conjuntos de teste", definir_conjuntos_testes },
		4 { "Gerar testes", gerar_testes },
		5 { "Sobre o AutomTest", mostrar_sobre },
		*/

		gtk_box_pack_start (GTK_BOX (box), button, TRUE, TRUE, 0);
	}

	gtk_widget_show_all (window_root);
	gtk_main ();

	return 0;
}
