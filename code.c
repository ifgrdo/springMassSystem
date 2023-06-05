#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <locale.h>
#include <gtk/gtk.h>

#if GTK_CHECK_VERSION(3,22,0)
gchar *cssData = "window {background-color: #FFFFF0 ;}\n"
#else
  gchar *cssData = "GtkWindow {background-color:  #FFFFF0}\n"
#endif
  "#label_1 {color: #483D8B; font-family: Calibri; font-weight: bold; font-size: 18px;}\n"
  "#label_2 {color: #696969; font-family: Calibri; font-weight: normal; font-size: 15px;}\n"
  "#label_3 {color: #696969; font-family: Calibri; font-weight: normal; font-size: 14px;}\n"
  ".bt_style1 label {background-color: darkslateblue; color: gainsboro; font-family: Calibri; font-weight: normal; font-size: 14px;}\n"
  ".bt_style2 label {background-color: darkred; color: ghostwhite; font-family: Calibri; font-weight: normal; font-size: 16px;}"
  ".bt_style3 label {background-color: lightslategray; color: ghostwhite; font-family: Calibri; font-weight: normal; font-size: 14px;}"
  ".box_style box {background-color: WhiteSmoke}";

gdouble x1_m, x2_m, x3_m, omega;
gdouble x1, x2, x3, v1, v2, v3;
gdouble k, m, F, w;
gdouble t = 0;
gdouble dt = 0.0001;
gint n_espira = 11;
gint c = 0, recomecar = 0, forma = 0;
gdouble escala_t1 = 45, escala_t2 = 45, escala_pos1 = 40, escala_pos2 = 40, escala_vel = 20;
gint contador=0;
gdouble r_m1 = 0.795, g_m1 = 0.169, b_m1 = 0.123;
gdouble r_m2 = 0.166, g_m2 = 0.398, b_m2 = 0.455;
gdouble r_m3 = 0.763, g_m3 = 0.665, b_m3 = 0.129;
gdouble vet_x1[1000], vet_x2[1000], vet_x3[1000], vet_v1[1000], vet_v2[1000], vet_v3[1000], vet_t[1000];
gchar pos_1[16] = "x1: ", pos_2[16] = "x2: ", pos_3[16] = "x3: ";
gchar vel_1[16] = "v1:  ", vel_2[16] = "v2: ", vel_3[16] = "v3: ";
gchar forca[16] = "F:  ";
gint comp_string;
GtkWidget *b_parar, *b_iniciar, *b_recomecar;
GtkWidget *draw_area1, *draw_area2, *draw_area3, *draw_area4;
GtkWidget *radio_button1, *radio_button2, *radio_button3, *radio_button4, *radio_button5, *radio_button6, *radio_button7;
GtkWidget *spin_m, *spin_k, *spin_a, *spin_p, *spin_pos_1, *spin_pos_2, *spin_pos_3, *spin_vel_1, *spin_vel_2, *spin_vel_3;
GtkWidget *switch1, *switch2, *switch3, *switch4, *switch5;

void
provider_create (gchar *chData)
{
  GtkCssProvider *provider ;
  GdkDisplay     *display  ;
  GdkScreen      *screen   ;

  provider = gtk_css_provider_new ();
  display = gdk_display_get_default ();
  screen = gdk_display_get_default_screen (display);

  gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider),
                                             GTK_STYLE_PROVIDER_PRIORITY_USER);
  
  gtk_css_provider_load_from_data (GTK_CSS_PROVIDER (provider), chData, -1, NULL);
  g_object_unref (provider);
}


GtkWidget *
create_spin_button_m (void)
{
  GtkAdjustment *adj_m;

  adj_m = gtk_adjustment_new (5, 0.5, 10, 0.1, 1, 0);
  spin_m = gtk_spin_button_new (adj_m, 0.1, 2);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spin_m), TRUE);
  gtk_widget_set_size_request (spin_m, 150, 50);

  return spin_m;
}

GtkWidget *
create_spin_button_k (void)
{
  GtkAdjustment *adj_k;

  adj_k = gtk_adjustment_new (20, 0.5, 50, 0.5, 1, 0);
  spin_k = gtk_spin_button_new (adj_k, 0.5, 2);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spin_k), TRUE);
  gtk_widget_set_size_request (spin_k, 150, 50);

  return spin_k;
}

GtkWidget *
create_spin_button_a (void)
{
  GtkAdjustment *adj_a;

  adj_a = gtk_adjustment_new (5, 0, 20, 0.5, 1, 0);
  spin_a = gtk_spin_button_new (adj_a, 0.5, 2);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spin_a), TRUE);
  gtk_widget_set_size_request (spin_a, 150, 50);

  return spin_a;
}

GtkWidget *
create_spin_button_p (void)
{
  GtkAdjustment *adj_p;

  adj_p = gtk_adjustment_new (1.5, 0, 5, 0.1, 1, 0);
  spin_p = gtk_spin_button_new (adj_p, 0.1, 2);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spin_p), TRUE);
  gtk_widget_set_size_request (spin_p, 150, 50);

  return spin_p;
}

GtkWidget *
create_spin_button_pos_1 (void)
{
  GtkAdjustment *adj_pos_1;

  adj_pos_1 = gtk_adjustment_new (0.5, -1.0, 3.0, 0.01, 1, 0);
  spin_pos_1 = gtk_spin_button_new (adj_pos_1, 0.01, 2);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spin_pos_1), TRUE);
  gtk_widget_set_size_request (spin_pos_1, 100, 40);

  return spin_pos_1;
}

GtkWidget *
create_spin_button_vel_1 (void)
{
  GtkAdjustment *adj_vel_1;

  adj_vel_1 = gtk_adjustment_new (1, -2.0, 2.0, 0.01, 1, 0);
  spin_vel_1 = gtk_spin_button_new (adj_vel_1, 0.01, 2);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spin_vel_1), TRUE);
  gtk_widget_set_size_request (spin_vel_1, 100, 40);

  return spin_vel_1;
}

GtkWidget *
create_spin_button_pos_2 (void)
{
  GtkAdjustment *adj_pos_2;

  adj_pos_2 = gtk_adjustment_new (1.5, -1.0, 3.0, 0.01, 1, 0);
  spin_pos_2 = gtk_spin_button_new (adj_pos_2, 0.01, 2);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spin_pos_2), TRUE);
  gtk_widget_set_size_request (spin_pos_2, 100, 40);

  return spin_pos_2;
}

GtkWidget *
create_spin_button_vel_2 (void)
{
  GtkAdjustment *adj_vel_2;

  adj_vel_2 = gtk_adjustment_new (0.5, -2.0, 2.0, 0.01, 1, 0);
  spin_vel_2 = gtk_spin_button_new (adj_vel_2, 0.01, 2);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spin_vel_2), TRUE);
  gtk_widget_set_size_request (spin_vel_2, 100, 40);

  return spin_vel_2;
}

GtkWidget *
create_spin_button_pos_3 (void)
{
  GtkAdjustment *adj_pos_3;

  adj_pos_3 = gtk_adjustment_new (2.0, -1.0, 3.0, 0.01, 1, 0);
  spin_pos_3 = gtk_spin_button_new (adj_pos_3, 0.01, 2);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spin_pos_3), TRUE);
  gtk_widget_set_size_request (spin_pos_3, 100, 40);

  return spin_pos_3;
}

GtkWidget *
create_spin_button_vel_3 (void)
{
  GtkAdjustment *adj_vel_3;

  adj_vel_3 = gtk_adjustment_new (-0.1, -2, 2, 0.01, 1, 0);
  spin_vel_3 = gtk_spin_button_new (adj_vel_3, 0.01, 2);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spin_vel_3), TRUE);
  gtk_widget_set_size_request (spin_vel_3, 100, 40);

  return spin_vel_3;
}

gboolean
espiras_mola (GtkWidget *widget)
{
  n_espira = gtk_range_get_value (GTK_RANGE (widget));

  return FALSE;
}

void
get_values (void)
{
  m = gtk_spin_button_get_value (GTK_SPIN_BUTTON (spin_m));
  k = gtk_spin_button_get_value (GTK_SPIN_BUTTON (spin_k));
  F = gtk_spin_button_get_value (GTK_SPIN_BUTTON (spin_a));
  w = gtk_spin_button_get_value (GTK_SPIN_BUTTON (spin_p));
  x1 = gtk_spin_button_get_value (GTK_SPIN_BUTTON (spin_pos_1));
  x2 = gtk_spin_button_get_value (GTK_SPIN_BUTTON (spin_pos_2));
  x3 = gtk_spin_button_get_value (GTK_SPIN_BUTTON (spin_pos_3));
  v1 = gtk_spin_button_get_value (GTK_SPIN_BUTTON (spin_vel_1));
  v2 = gtk_spin_button_get_value (GTK_SPIN_BUTTON (spin_vel_2));
  v3 = gtk_spin_button_get_value (GTK_SPIN_BUTTON (spin_vel_3));
  }
  
gboolean
obtain_pos_and_vel ()
{
  int j;
  omega = k/m; //assim já fica elevado ao quadrado

  if(c==0){

    vet_x1[contador] = x1;
    vet_x2[contador] = x2;
    vet_x3[contador] = x3;
    vet_v1[contador] = v1;
    vet_v2[contador] = v2;
    vet_v3[contador] = v3;
    vet_t[contador] = t;

    sprintf(&pos_1[comp_string], "%7.3lf m", x1);
    sprintf(&pos_2[comp_string], "%7.3lf m", x2);
    sprintf(&pos_3[comp_string], "%7.3lf m", x3);
    sprintf(&vel_1[comp_string], "%7.3lf m/s", v1);
    sprintf(&vel_2[comp_string], "%7.3lf m/s", v2);
    sprintf(&vel_3[comp_string], "%7.3lf m/s", v3);
    sprintf(&forca[comp_string], "%7.3lf N", F*cos(w*t));

    //printf("t: %lf   x1: %lf  x2: %lf  x3: %lf  v1: %lf  v2: %lf  v3: %lf\n", t, x1, x2, x3, v1, v2, v3);

    if(contador < 1000)
      ++contador;
    
    for(j=0; j<100; ++j){

      x1_m = x1;
      x2_m = x2;
      x3_m = x3;

      v1 = v1 + (omega*(x2_m - 2*x1_m) + (F/m)*cos(w*t))*dt;
      x1 = x1 + v1*dt;
    
      v2 = v2 + omega*(x1_m - 2*x2_m + x3_m)*dt;
      x2 = x2 + v2*dt;
    
      v3 = v3 + omega*(x2_m - 2*x3_m)*dt;
      x3 = x3 + v3*dt;

      t=t+dt;
    }

  
    if(contador==1000){
      for(j=0; j<999; ++j)
	{
	  vet_x1[j] = vet_x1[j+1];
	  vet_x2[j] = vet_x2[j+1];
	  vet_x3[j] = vet_x3[j+1];
	  vet_v1[j] = vet_v1[j+1];
	  vet_v2[j] = vet_v2[j+1];
	  vet_v3[j] = vet_v3[j+1];
	}


      --contador;
    }
  }
  
  if ((!GTK_IS_WIDGET (draw_area1)) || (!gtk_widget_get_window (draw_area1)))
    return FALSE;

  gtk_widget_queue_draw (draw_area1);

  return TRUE;
}

gboolean
draw_function (GtkWidget *widget,
	       cairo_t *cr,
	       gpointer data)
{
  gdouble equil_1, equil_2, equil_3;
  gdouble pos1, pos2, pos3;
  gdouble pos_r1, pos_r2, pos_r3;
  gdouble r_width, r_height, raio;
  gdouble h_min, h_max, h_medio;
  gint dir1, dir2, dir3, dir4;
  gint i;
  GtkAllocation alloc1;

  gtk_widget_get_allocation (draw_area1, &alloc1);

  //Função que resolve as equações diferenciais
  obtain_pos_and_vel (widget);

  //Título
  cairo_move_to (cr, alloc1.width/2-50, 25);
  cairo_select_font_face (cr, "Calibri", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_font_size (cr, 18);
  cairo_set_source_rgb (cr, 0.263, 0.263, 0.263);
  cairo_show_text (cr, "Simulação");
  cairo_stroke (cr);

  //Coordenada y em que se centram as massas e as molas
  h_medio = 125;
  
  //Extremidades
  cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
  cairo_rectangle (cr, 10, h_medio-75, 5, 150);
  cairo_rectangle (cr, alloc1.width-15, h_medio-75, 5, 150);
  cairo_fill (cr);
  cairo_stroke (cr);

  //Comprimento e altura dos retângulos
  r_width = 85;
  r_height = 15+7*m;
  raio = 5+3.75*m;

  //Posição do lado esquerdo dos retângulos em situação de equilíbrio
  equil_1 = alloc1.width/4;
  equil_2 = alloc1.width/2;
  equil_3 = 3*(alloc1.width/4);
  
  //Posições do centro de cada massa após deslocamento
  pos1 = equil_1 + x1*25;
  pos2 = equil_2 + x2*25;
  pos3 = equil_3 + x3*25;

  //Posição do lado esquerdo dos retângulos após deslocamento
  pos_r1 = pos1 - r_width/2;
  pos_r2 = pos2 - r_width/2;
  pos_r3 = pos3 - r_width/2;
  
  //Massa1
  cairo_set_source_rgb (cr, r_m1, g_m1, b_m1);
  if(forma==0){
    cairo_rectangle (cr, pos_r1, h_medio-(r_height/2), r_width, r_height);
    cairo_fill(cr);
  }

  if(forma==1){
    cairo_arc (cr, pos1, h_medio, raio, 0, 2*M_PI);
    cairo_fill (cr);
    cairo_arc (cr, pos1, h_medio, r_width/2, 0, 2*M_PI);
  }
  
  cairo_stroke(cr);

  //Massa2
  cairo_set_source_rgb (cr, r_m2, g_m2, b_m2);
  if(forma==0){
    cairo_rectangle (cr, pos_r2, h_medio-(r_height/2), r_width, r_height);
    cairo_fill(cr);
  }

  if(forma==1){
    cairo_arc (cr, pos2, h_medio, raio, 0, 2*M_PI);
    cairo_fill (cr);
    cairo_arc (cr, pos2, h_medio, r_width/2, 0, 2*M_PI);
  }

  cairo_stroke(cr);

  //Massa3
  cairo_set_source_rgb (cr, r_m3, g_m3, b_m3);
  if(forma==0){
    cairo_rectangle (cr, pos_r3, h_medio-(r_height/2), r_width, r_height);
    cairo_fill(cr);
  }

  if(forma==1){
    cairo_arc (cr, pos3, h_medio, raio, 0, 2*M_PI);
    cairo_fill (cr);
    cairo_arc (cr, pos3, h_medio, r_width/2, 0, 2*M_PI);
  }
  cairo_stroke(cr);

  //Labels
  cairo_select_font_face (cr, "Calibri", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_source_rgb (cr, 0.263, 0.263, 0.263);
  cairo_set_font_size (cr, 16);

  cairo_move_to (cr, pos_r1+5, h_medio - 70);
  cairo_show_text (cr, "Massa 1");

  cairo_move_to (cr, pos_r2+5, h_medio - 70);
  cairo_show_text (cr, "Massa 2");

  cairo_move_to (cr, pos_r3+5, h_medio - 70);
  cairo_show_text (cr, "Massa 3");
  
  cairo_stroke(cr);
  
  //Valores da posição, da velocidade e da força F
  if(gtk_switch_get_active (GTK_SWITCH (switch1))==TRUE){

  cairo_select_font_face (cr, "Calibri", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
  cairo_set_font_size (cr, 14);

  cairo_move_to (cr, 3*(alloc1.width/16), 4*(alloc1.height)/5);
  cairo_show_text (cr, pos_1);
  cairo_move_to (cr, 15*(alloc1.width/32), 4*(alloc1.height)/5);
  cairo_show_text(cr, pos_2);
  cairo_move_to (cr, 3*(alloc1.width/4), 4*(alloc1.height)/5);
  cairo_show_text (cr, pos_3);

  cairo_move_to (cr, 3*(alloc1.width/16), 13*(alloc1.height)/15);
  cairo_show_text (cr, vel_1);
  cairo_move_to (cr, 15*(alloc1.width/32), 13*(alloc1.height)/15);
  cairo_show_text(cr, vel_2);
  cairo_move_to (cr, 3*(alloc1.width/4), 13*(alloc1.height)/15);
  cairo_show_text (cr, vel_3);

  cairo_move_to (cr, 3*(alloc1.width/16), 14*(alloc1.height)/15);
  cairo_show_text (cr, forca);

  cairo_stroke (cr);
}

  ///Posição de equilíbrio e vetores x

  if(gtk_switch_get_active (GTK_SWITCH (switch5))==TRUE){
  cairo_set_line_width (cr, 1.0);
  cairo_set_source_rgb (cr, 0, 0, 0);
  cairo_move_to (cr, equil_1, h_medio - 45);
  cairo_line_to (cr, equil_1, h_medio + 45);
  cairo_move_to (cr, equil_2, h_medio - 45);
  cairo_line_to (cr, equil_2, h_medio + 45);
  cairo_move_to (cr, equil_2, h_medio - 45);
  cairo_line_to (cr, equil_2, h_medio + 45);
  cairo_move_to (cr, equil_3, h_medio - 45);
  cairo_line_to (cr, equil_3, h_medio + 45);

  cairo_select_font_face (cr, "Calibri", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_source_rgb (cr, 0.263, 0.263, 0.263);
  cairo_set_font_size (cr, 13);

  cairo_move_to (cr, equil_1 - 35, h_medio + 58);
  cairo_show_text (cr, "Equilíbrio 1");
  cairo_move_to (cr, equil_2 - 35, h_medio + 58);
  cairo_show_text (cr, "Equilíbrio 2");
  cairo_move_to (cr, equil_3 - 35, h_medio + 58);
  cairo_show_text (cr, "Equilíbrio 3");

  cairo_stroke (cr);

  cairo_set_line_width (cr, 2.0);
  cairo_select_font_face (cr, "Calibri", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
  cairo_set_source_rgb (cr, 0.263, 0.263, 0.263);

  if(x1>0)
    dir1 = 2;

  else{
    if (x1<0)
      dir1 = -2;

    else
      dir1 = 0;
      
    }

  if(x2>0)
    dir2 = 2;

  else{
    if (x2<0)
      dir2 = -2;

    else
      dir2 = 0;
  }

  if(x3>0)
    dir3 = 2;

  else{
    if (x3<0)
      dir3 = -2;

    else
      dir3 = 0;
  }

  cairo_move_to (cr, equil_1, h_medio - 60);
  cairo_line_to (cr, equil_1, h_medio - 50);
  cairo_move_to (cr, equil_1, h_medio - 55);
  cairo_line_to (cr, pos1, h_medio - 55);
  cairo_move_to (cr, pos1, h_medio -55);
  cairo_line_to (cr, pos1 - dir1, h_medio - 60);
  cairo_move_to (cr, pos1, h_medio - 55);
  cairo_line_to (cr, pos1 - dir1, h_medio - 50);
  cairo_move_to (cr, pos1, h_medio - 40);
  cairo_show_text (cr, "x1");

  cairo_move_to (cr, equil_2, h_medio - 60);
  cairo_line_to (cr, equil_2, h_medio - 50);
  cairo_move_to (cr, equil_2, h_medio - 55);
  cairo_line_to (cr, pos2, h_medio - 55);
  cairo_move_to (cr, pos2, h_medio -55);
  cairo_line_to (cr, pos2 - dir2, h_medio - 60);
  cairo_move_to (cr, pos2, h_medio - 55);
  cairo_line_to (cr, pos2 - dir2, h_medio - 50);
  cairo_move_to (cr, pos2, h_medio - 40);
  cairo_show_text (cr, "x2");

  cairo_move_to (cr, equil_3, h_medio - 60);
  cairo_line_to (cr, equil_3, h_medio - 50);
  cairo_move_to (cr, equil_3, h_medio - 55);
  cairo_line_to (cr, pos3, h_medio - 55);
  cairo_move_to (cr, pos3, h_medio -55);
  cairo_line_to (cr, pos3 - dir3, h_medio - 60);
  cairo_move_to (cr, pos3, h_medio - 55);
  cairo_line_to (cr, pos3 - dir3, h_medio - 50);
  cairo_move_to (cr, pos3, h_medio - 40);
  cairo_show_text (cr, "x3");

  cairo_stroke (cr);
}

  //Vetor velocidade
  //Se v1>0, o vetor aponta para a direita; se v1<0, o vetor aponta para a esquerda
  
  //NOTA: 125 corresponde à coordenada y do centro das massas e da parte horizontal das molas
if(gtk_switch_get_active (GTK_SWITCH (switch2))==TRUE){
  cairo_set_line_width (cr, 2.0);
  cairo_select_font_face (cr, "Calibri", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
  cairo_set_source_rgb (cr, 0.263, 0.263, 0.263);

  cairo_move_to (cr, pos1, h_medio + 60);
  cairo_line_to (cr, pos1, h_medio + 70);
  cairo_move_to (cr, pos1, h_medio + 65);
  cairo_line_to (cr, pos1 + 20*v1, h_medio + 65);
  cairo_move_to (cr, pos1 + 20*v1, h_medio + 65);
  cairo_line_to (cr, pos1 + 17.5*v1, h_medio + 60);
  cairo_move_to (cr, pos1 + 20*v1, h_medio + 65);
  cairo_line_to (cr, pos1 + 17.5*v1, h_medio + 70);
  cairo_move_to (cr, pos1 + 20*v1, h_medio + 80);
  cairo_show_text (cr, "v1");
    
  cairo_move_to (cr, pos2, h_medio + 60);
  cairo_line_to (cr, pos2, h_medio + 70); 
  cairo_move_to (cr, pos2, h_medio + 65);
  cairo_line_to (cr, pos2 + 20*v2, h_medio + 65);
  cairo_move_to (cr, pos2 + 20*v2, h_medio + 65);
  cairo_line_to (cr, pos2 + 17.5*v2, h_medio + 60);
  cairo_move_to (cr, pos2 + 20*v2, h_medio + 65);
  cairo_line_to (cr, pos2 + 17.5*v2, h_medio + 70);
  cairo_move_to (cr, pos2 + 20*v2, h_medio + 80);
  cairo_show_text (cr, "v2");
    
  cairo_move_to (cr, pos3, h_medio + 60);
  cairo_line_to (cr, pos3, h_medio + 70);
  cairo_move_to (cr, pos3, h_medio + 65);
  cairo_line_to (cr, pos3 + 20*v3, h_medio + 65);
  cairo_move_to (cr, pos3 + 20*v3, h_medio + 65);
  cairo_line_to (cr, pos3 + 17.5*v3, h_medio + 60);
  cairo_move_to (cr, pos3 + 20*v3, h_medio + 65);
  cairo_line_to (cr, pos3 + 17.5*v3, h_medio + 70);
  cairo_move_to (cr, pos3 + 20*v3, h_medio + 80);
  cairo_show_text (cr, "v3");
  cairo_stroke (cr);
}

  //Vetor força que atua na massa 1
  cairo_set_line_width (cr, 2.0);
  cairo_select_font_face (cr, "Calibri", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
  cairo_set_source_rgb (cr, r_m1, g_m1, b_m1);

if(gtk_switch_get_active(GTK_SWITCH (switch3))==TRUE){



  if(F!=0){
  cairo_move_to (cr, pos1, h_medio + 85);
  cairo_line_to (cr, pos1, h_medio + 95);
  cairo_move_to (cr, pos1, h_medio + 90);
  cairo_line_to (cr, pos1 + 10*(F*cos(w*t)), h_medio + 90);

  if(F*cos(w*t)>0)
    dir4 = 2;

  else
    dir4 = -2;

  cairo_move_to (cr, pos1 + 10*(F*cos(w*t)), h_medio + 90);
  cairo_line_to (cr, pos1 + 10*(F*cos(w*t)) - dir4, h_medio + 85);
  cairo_move_to (cr, pos1 + 10*(F*cos(w*t)), h_medio + 90);
  cairo_line_to (cr, pos1 + 10*(F*cos(w*t)) - dir4, h_medio + 95);

  cairo_move_to (cr, pos1 + 10*(F*cos(w*t)), h_medio + 105);
  cairo_show_text (cr, "F");
}

if(F==0){
  cairo_move_to (cr, pos1 - 18, h_medio + 92);
  cairo_show_text (cr, "F= 0 N");
 }
}

 cairo_stroke (cr);

  //Molas

  h_min = h_medio-30;
  h_max = h_medio+30;
  
  //Desenho
  cairo_set_line_width (cr, 1);
  cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);

  //Forma 1

  //Mola 1
  cairo_move_to(cr, 15, h_medio);
  cairo_line_to (cr, 40, h_medio);
  cairo_line_to (cr, 40 + ((pos_r1-65)/(n_espira*2)), h_min);

  for(i=1; i<=(n_espira-1)/2; ++i){
    cairo_line_to (cr, 40 + (4*i-1)*((pos_r1-65)/(n_espira*2)), h_max);
    cairo_line_to (cr, 40 + (4*i+1)*((pos_r1-65)/(n_espira*2)), h_min);
    }
  
  cairo_line_to (cr, 40 + (n_espira*2)*((pos_r1-65)/(n_espira*2)), h_medio);
  cairo_line_to (cr, pos_r1, h_medio);


  //Mola 2
  cairo_move_to (cr, pos_r1+r_width, h_medio);
  cairo_line_to (cr, pos_r1+r_width+25, h_medio);
  cairo_line_to (cr, pos_r1+r_width+25 + ((pos_r2-pos_r1-135)/(n_espira*2)), h_min);

  for(i=1; i<=(n_espira-1)/2; ++i){
    cairo_line_to (cr, pos_r1+r_width+25 + (4*i-1)*((pos_r2-pos_r1-135)/(n_espira*2)), h_max);
    cairo_line_to (cr, pos_r1+r_width+25 + (4*i+1)*((pos_r2-pos_r1-135)/(n_espira*2)), h_min);
  }
    
  cairo_line_to (cr, pos_r1+r_width+25 + (n_espira*2)*((pos_r2-pos_r1-135)/(n_espira*2)), h_medio);
  cairo_line_to (cr, pos_r2, h_medio);

  
  //Mola 3
  cairo_move_to (cr, pos_r2+r_width, h_medio);
  cairo_line_to (cr, pos_r2+r_width+25, h_medio);
  cairo_line_to (cr, pos_r2+r_width+25 + ((pos_r3-pos_r2-135)/(n_espira*2)), h_min);

  for(i=1; i<=(n_espira-1)/2; ++i){
    cairo_line_to (cr, pos_r2+r_width+25 + (4*i-1)*((pos_r3-pos_r2-135)/(n_espira*2)), h_max);
    cairo_line_to (cr, pos_r2+r_width+25 + (4*i+1)*((pos_r3-pos_r2-135)/(n_espira*2)), h_min);
  }
  
  cairo_line_to (cr, pos_r2+r_width+25 + (n_espira*2)*((pos_r3-pos_r2-135)/(n_espira*2)), h_medio);
  cairo_line_to (cr, pos_r3, h_medio);

  
  //Mola 4 
  cairo_move_to (cr, pos_r3+r_width, h_medio);
  cairo_line_to (cr, pos_r3+r_width+25, h_medio);
  cairo_line_to (cr, pos_r3+r_width+25 + ((alloc1.width-pos_r3-150)/(n_espira*2)), h_min);

  for(i=1; i<=(n_espira-1)/2; ++i){
    cairo_line_to (cr, pos_r3+r_width+25 + (4*i-1)*((alloc1.width-pos_r3-150)/(n_espira*2)), h_max);
    cairo_line_to (cr, pos_r3+r_width+25 + (4*i+1)*((alloc1.width-pos_r3-150)/(n_espira*2)), h_min);
  }

  cairo_line_to (cr, pos_r3+r_width+25 + (n_espira*2)*((alloc1.width-pos_r3-150)/(n_espira*2)), h_medio);
  cairo_line_to (cr, alloc1.width-15, h_medio);
  
  cairo_stroke (cr);

  /*//Forma 2

  //Mola 1
  cairo_move_to(cr, 15, h_medio);
  cairo_line_to (cr, 15 + ((pos1-15)/(n_espira*2)), h_min);

  for(i=1; i<=(n_espira-1)/2; ++i){
    cairo_line_to (cr, 15 + (4*i-1)*((pos1-15)/(n_espira*2)), h_max);
    cairo_line_to (cr, 15 + (4*i+1)*((pos1-15)/(n_espira*2)), h_min);
    }
  
  cairo_line_to (cr, pos1, h_medio);


  //Mola 2
  cairo_move_to (cr, pos1+r_width, h_medio);
  cairo_line_to (cr, pos1+r_width + ((pos2-pos1-85)/(n_espira*2)), h_min);

  for(i=1; i<=(n_espira-1)/2; ++i){
    cairo_line_to (cr, pos1+r_width + (4*i-1)*((pos2-pos1-85)/(n_espira*2)), h_max);
    cairo_line_to (cr, pos1+r_width + (4*i+1)*((pos2-pos1-85)/(n_espira*2)), h_min);
  }
    
  cairo_line_to (cr, pos2, h_medio);

  
  //Mola 3
  cairo_move_to (cr, pos2+r_width, h_medio);
  cairo_line_to (cr, pos2+r_width + ((pos3-pos2-85)/(n_espira*2)), h_min);

  for(i=1; i<=(n_espira-1)/2; ++i){
    cairo_line_to (cr, pos2+r_width + (4*i-1)*((pos3-pos2-85)/(n_espira*2)), h_max);
    cairo_line_to (cr, pos2+r_width + (4*i+1)*((pos3-pos2-85)/(n_espira*2)), h_min);
  }
  
  cairo_line_to (cr, pos3, h_medio);

  
  //Mola 4 
  cairo_move_to (cr, pos3+r_width, h_medio);
  cairo_line_to (cr, pos3+r_width+ + ((alloc.width-pos3-100)/(n_espira*2)), h_min);

  for(i=1; i<=(n_espira-1)/2; ++i){
    cairo_line_to (cr, pos3+r_width + (4*i-1)*((alloc.width-pos3-100)/(n_espira*2)), h_max);
    cairo_line_to (cr, pos3+r_width + (4*i+1)*((alloc.width-pos3-100)/(n_espira*2)), h_min);
  }

  cairo_line_to (cr, alloc.width-15, h_medio);
  
  cairo_stroke (cr);*/

  return FALSE;
}

gboolean
time_h (GtkWidget *widget)
{
  if(!GTK_IS_WIDGET(widget) || !gtk_widget_get_window (widget))
      return FALSE;

  gtk_widget_queue_draw (widget);

  return TRUE;
}

//Escala horizontal do gráfico posição-tempo
gboolean
escala_tempo1 (GtkWidget *widget)
{
  escala_t1 = gtk_range_get_value (GTK_RANGE (widget));
  printf("escala t1: %lf\n", escala_t1);

  return FALSE;
}

//Escala vertical do gráfico posição-tempo
gboolean
escala_posicao1 (GtkWidget *widget)
{
  escala_pos1 = gtk_range_get_value (GTK_RANGE (widget));
  printf("escala pos: %lf\n", escala_pos1);

  return FALSE;
}

//Escala horizontal do gráfico velocidade_tempo
gboolean
escala_tempo2 (GtkWidget *widget)
{
  escala_t2 = gtk_range_get_value (GTK_RANGE (widget));
  //printf("escala t2: %lf\n", escala_t2);

  return FALSE;
}

//Escala vertical do gráfico velocidade-tempo e do gráfico velocidade-posição
gboolean
escala_velocidade (GtkWidget *widget)
{
  escala_vel = gtk_range_get_value (GTK_RANGE (widget));
  //printf("escala vel: %lf\n", escala_vel);

  return FALSE;
}

//Escala horizontal do gráfico velocidade-posição
gboolean
escala_posicao2 (GtkWidget *widget)
{
  escala_pos2 = gtk_range_get_value (GTK_RANGE (widget));

  return FALSE;
}

gboolean
button_parar (GtkWidget *widget)
{
  if(c==0){
    gtk_button_set_label (GTK_BUTTON (widget), "Continuar");
    ++c;
  }

  else
    if(c==1){
      gtk_button_set_label (GTK_BUTTON (widget), "Parar");
      --c;
    }
    
  return FALSE;
}


gboolean
button_iniciar (GtkWidget *widget)
{
  int i;
  c = 0;
  contador = 0;
  t = 0;

  gtk_button_set_label (GTK_BUTTON (b_parar), "Parar");

  get_values ();

  return FALSE;
}


gboolean
button_recomecar (GtkWidget *widget)
{
  int j;
  c=0;
  
  gtk_spin_button_set_value (GTK_SPIN_BUTTON (spin_m), 5.00);
  gtk_spin_button_set_value (GTK_SPIN_BUTTON (spin_k), 20.00);
  gtk_spin_button_set_value (GTK_SPIN_BUTTON (spin_a), 5.00);
  gtk_spin_button_set_value (GTK_SPIN_BUTTON (spin_p), 1.50);
  gtk_spin_button_set_value (GTK_SPIN_BUTTON (spin_pos_1), 0.50);
  gtk_spin_button_set_value (GTK_SPIN_BUTTON (spin_vel_1), 1.00);
  gtk_spin_button_set_value (GTK_SPIN_BUTTON (spin_pos_2), 1.50);
  gtk_spin_button_set_value (GTK_SPIN_BUTTON (spin_vel_2), 0.50);
  gtk_spin_button_set_value (GTK_SPIN_BUTTON (spin_pos_3), 2.00);
  gtk_spin_button_set_value (GTK_SPIN_BUTTON (spin_vel_3), -0.10);

  r_m1 = 0.795;
  g_m1 = 0.169;
  b_m1 = 0.123;
  r_m2 = 0.166;
  g_m2 = 0.398;
  b_m2 = 0.455;
  r_m3 = 0.763;
  g_m3 = 0.665;
  b_m3 = 0.129;

  contador = 0;
  t = 0;

  gtk_button_set_label (GTK_BUTTON (b_parar), "Parar");

  get_values ();

  return FALSE;
}


gboolean
graf_posicao_tempo (GtkWidget *widget,
		    cairo_t *cr,
		    gpointer data)
{
  GtkAllocation alloc;
  int j, inc = 1;
  double num;

  gtk_widget_get_allocation (draw_area2, &alloc);

  //Título
  cairo_move_to (cr, 0.3*alloc.width, 25);
  cairo_select_font_face (cr, "Calibri", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_font_size (cr, 18);
  cairo_set_source_rgb (cr, 0.263, 0.263, 0.263);
  cairo_show_text (cr, "Gráfico posição-tempo - x(t)");
  cairo_stroke (cr);

  cairo_set_source_rgb (cr, 0, 0, 0);
  cairo_set_line_width (cr, 1.0);

  //Eixo horizontal
  cairo_move_to (cr, 0, alloc.height/2);
  cairo_line_to (cr, alloc.width, alloc.height/2);
  cairo_move_to (cr, alloc.width, alloc.height/2);
  cairo_line_to (cr, alloc.width - 5, (alloc.height/2)+5);
  cairo_move_to (cr, alloc.width, alloc.height/2);
  cairo_line_to (cr, alloc.width - 5, (alloc.height/2)-5);

  cairo_stroke (cr);

  //Eixo vertical
  cairo_move_to (cr, 0, alloc.height/10);
  cairo_line_to (cr, 0, 9*(alloc.height/10));
  cairo_move_to (cr, 0, alloc.height/10);
  cairo_line_to (cr,-5, (alloc.height/10)+5);
  cairo_move_to (cr, 0, alloc.height/10);
  cairo_line_to (cr, 5, (alloc.height/10)+5);

  cairo_stroke (cr);

  //Marcações

  if(gtk_switch_get_active (GTK_SWITCH (switch4))==TRUE){

  
    num = 450/escala_t1;

    cairo_set_line_width (cr, 1);
    cairo_set_source_rgb (cr, 0, 0, 0);

    for(j=1; j<=(int)num; ++j){
     cairo_move_to (cr, inc * (alloc.width/num), alloc.height/2 - 5);
     cairo_line_to (cr, inc * (alloc.width/num), alloc.height/2 + 5);

     ++inc;
    }

    cairo_stroke (cr);

    cairo_set_line_width (cr, 2);
    cairo_move_to (cr, 0, 9*(alloc.height/10) - 20);
    cairo_line_to (cr, alloc.width/num, 9*(alloc.height/10) - 20);
    cairo_move_to (cr, 0, 9*(alloc.height/10));
    cairo_select_font_face (cr, "Calibri", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size (cr, 14);
    cairo_show_text (cr, "1 segundo");

    cairo_move_to (cr, 0, 9*(alloc.height/10) - 20);
    cairo_line_to (cr, 5, 9*(alloc.height/10) - 25);
    cairo_move_to (cr, 0, 9*(alloc.height/10) - 20);
    cairo_line_to (cr, 5, 9*(alloc.height/10) - 15);

    cairo_move_to (cr, alloc.width/num, 9*(alloc.height/10) - 20);
    cairo_line_to (cr, alloc.width/num - 5, 9*(alloc.height/10) - 25);
    cairo_move_to (cr, alloc.width/num, 9*(alloc.height/10) - 20);
    cairo_line_to (cr, alloc.width/num - 5, 9*(alloc.height/10) - 15);
    cairo_stroke (cr);
  }
  
  //Identificação dos eixos
  cairo_set_font_size (cr, 13);
  cairo_select_font_face (cr, "Calibri", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
  cairo_move_to (cr, 2, (alloc.height/2)+12);
  cairo_show_text (cr, "0");
  cairo_move_to (cr, alloc.width-22, (alloc.height/2)+17);
  cairo_show_text (cr, "t(s)");
  cairo_move_to (cr, 5, (alloc.height/10)+5);
  cairo_show_text (cr, "x(m)");
  
  cairo_stroke (cr);

  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (radio_button6)) == TRUE){

    //Gráfico
    //Massa1
    cairo_set_line_width (cr, 3.0);
    cairo_set_source_rgb (cr, r_m1, g_m1, b_m1);

      cairo_move_to (cr, alloc.width * (1-(escala_t1/45)), alloc.height/2 - vet_x1[0] * escala_pos1);

      for (j=0; j<contador; ++j)
	      {
            cairo_line_to (cr, alloc.width * (1-(escala_t1/45)) + escala_t1 * vet_t[j], alloc.height/2 - vet_x1[j] * escala_pos1);
	      }  
    
    cairo_stroke(cr);

 //Massa2
    cairo_set_source_rgb (cr, r_m2, g_m2, b_m2);
    cairo_move_to (cr, alloc.width * (1-(escala_t1/45)), alloc.height/2 - vet_x2[0] * escala_pos1);
    for (j=0; j<contador-1; ++j)
      {
	       cairo_line_to (cr, alloc.width * (1-(escala_t1/45)) + escala_t1 * vet_t[j], alloc.height/2 - vet_x2[j] * escala_pos1);
      }

    cairo_stroke(cr);

    //Massa3
    cairo_set_source_rgb (cr, r_m3, g_m3, b_m3);
    cairo_move_to (cr, alloc.width * (1-(escala_t1/45)), alloc.height/2 - vet_x3[0] * escala_pos1);
    for (j=0; j<contador-1; ++j)
      {
	       cairo_line_to (cr, alloc.width * (1-(escala_t1/45)) + escala_t1 * vet_t[j], alloc.height/2 - vet_x3[j] * escala_pos1);
	    }

    cairo_stroke (cr);
  }
  
  return FALSE;
}

gboolean
graf_velocidade_tempo (GtkWidget *widget,
		       cairo_t *cr,
		       gpointer data)
{
  int j;
  gint inc = 1;
  gdouble num;
  GtkAllocation alloc;

  gtk_widget_get_allocation (draw_area3, &alloc);

  //Título
  cairo_move_to (cr, 100, 25);
  cairo_select_font_face (cr, "Calibri", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_font_size (cr, 18);
  cairo_set_source_rgb (cr, 0.263, 0.263, 0.263);
  cairo_show_text (cr, "Gráfico velocidade-tempo - v(t)");
  cairo_stroke (cr);

  cairo_set_source_rgb (cr, 0, 0, 0);
  cairo_set_line_width (cr, 1.0);

  //Eixo horizontal
  cairo_move_to (cr, 0, alloc.height/2);
  cairo_line_to (cr, alloc.width, alloc.height/2);
  cairo_move_to (cr, alloc.width, alloc.height/2);
  cairo_line_to (cr, (alloc.width)-5, (alloc.height/2)+5);
  cairo_move_to (cr, alloc.width, alloc.height/2);
  cairo_line_to (cr, (alloc.width)-5, (alloc.height/2)-5);
  
  cairo_stroke (cr);
  
  //Eixo vertical
  cairo_move_to (cr, 0, alloc.height/10);
  cairo_line_to (cr, 0, 9*(alloc.height/10));
  cairo_move_to (cr, 0, alloc.height/10);
  cairo_line_to (cr, -5, (alloc.height/10)+5);
  cairo_move_to (cr, 0, alloc.height/10);
  cairo_line_to (cr, 5, (alloc.height/10)+5);

  cairo_stroke (cr);

  //Identificação dos eixos
  cairo_set_font_size (cr, 13);
  cairo_select_font_face (cr, "Calibri", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
  cairo_move_to (cr, 2, (alloc.height/2)+12);
  cairo_show_text (cr, "0");
  cairo_move_to (cr, alloc.width-22, (alloc.height/2)+17);
  cairo_show_text (cr, "t(s)");
  cairo_move_to (cr, 5, (alloc.height/10)+5);
  cairo_show_text (cr, "v(m/s)");
  
  cairo_stroke (cr);

  //Marcações
  num = 450/escala_t2;

    cairo_set_line_width (cr, 1);
    cairo_set_source_rgb (cr, 0, 0, 0);

    for(j=1; j<=(int)num; ++j){
     cairo_move_to (cr, inc * (alloc.width/num), alloc.height/2 - 5);
     cairo_line_to (cr, inc * (alloc.width/num), alloc.height/2 + 5);

     ++inc;
   }
    cairo_stroke (cr);

    cairo_set_line_width (cr, 2);
    cairo_move_to (cr, 0, 9*(alloc.height/10) - 20);
    cairo_line_to (cr, alloc.width/num, 9*(alloc.height/10) - 20);
    cairo_move_to (cr, 0, 9*(alloc.height/10));
    cairo_select_font_face (cr, "Calibri", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size (cr, 14);
    cairo_show_text (cr, "1 segundo");

    cairo_move_to (cr, 0, 9*(alloc.height/10) - 20);
    cairo_line_to (cr, 5, 9*(alloc.height/10) - 25);
    cairo_move_to (cr, 0, 9*(alloc.height/10) - 20);
    cairo_line_to (cr, 5, 9*(alloc.height/10) - 15);

    cairo_move_to (cr, alloc.width/num, 9*(alloc.height/10) - 20);
    cairo_line_to (cr, alloc.width/num - 5, 9*(alloc.height/10) - 25);
    cairo_move_to (cr, alloc.width/num, 9*(alloc.height/10) - 20);
    cairo_line_to (cr, alloc.width/num - 5, 9*(alloc.height/10) - 15);
    cairo_stroke (cr);

  //Gráfico

  cairo_set_line_width (cr, 3.0);
      
  //Massa1

  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (radio_button1)) == TRUE)
    {
      cairo_set_source_rgb (cr, r_m1, g_m1, b_m1);
      cairo_move_to (cr, alloc.width * (1-(escala_t2/45)), alloc.height/2 - vet_v1[0] * escala_vel);
      for (j=0; j<contador; ++j)
	     {
	        cairo_line_to (cr, alloc.width * (1-(escala_t2/45)) + escala_t2 * vet_t[j], alloc.height/2 - vet_v1[j] * escala_vel);
	     } 

      cairo_stroke (cr);
    }

  //Massa2

  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (radio_button2)) == TRUE)
    {
      cairo_set_source_rgb (cr, r_m2, g_m2, b_m2);
      cairo_move_to (cr, alloc.width * (1-(escala_t2/45)), alloc.height/2 - vet_v2[0] * escala_vel);

      for (j=0; j<contador; ++j)
	{
	  cairo_line_to (cr, alloc.width * (1-(escala_t2/45)) + escala_t2 * vet_t[j], alloc.height/2 - vet_v2[j] * escala_vel);
	}

      cairo_stroke (cr);
    }

  //Massa3

  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (radio_button3)) == TRUE)
    {
      cairo_set_source_rgb (cr, r_m3, g_m3, b_m3);
      cairo_move_to (cr, alloc.width * (1-(escala_t2/45)), alloc.height/2 - vet_v3[0] * escala_vel);
      for (j=0; j<contador; ++j)
	{
	  cairo_line_to (cr, alloc.width * (1-(escala_t2/45)) + escala_t2 * vet_t[j], alloc.height/2 - vet_v3[j] * escala_vel);
	}
     cairo_stroke (cr);
    }

  //Massas 1, 2 e 3

  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (radio_button4)) == TRUE)
    {
      cairo_set_source_rgb (cr, r_m1, g_m1, b_m1);
      cairo_move_to (cr, alloc.width * (1-(escala_t2/45)), alloc.height/2 - vet_v1[0] * escala_vel);
      for (j=0; j<contador; ++j)
       {
          cairo_line_to (cr, alloc.width * (1-(escala_t2/45)) + escala_t2 * vet_t[j], alloc.height/2 - vet_v1[j] * escala_vel);
       } 

        cairo_stroke (cr);


      cairo_set_source_rgb (cr, r_m2, g_m2, b_m2);
      cairo_move_to (cr, alloc.width * (1-(escala_t2/45)), alloc.height/2 - vet_v2[0] * escala_vel);

      for (j=0; j<contador; ++j)
  {
    cairo_line_to (cr, alloc.width * (1-(escala_t2/45)) + escala_t2 * vet_t[j], alloc.height/2 - vet_v2[j] * escala_vel);
  }

      cairo_stroke (cr);

      cairo_set_source_rgb (cr, r_m3, g_m3, b_m3);
      cairo_move_to (cr, alloc.width * (1-(escala_t2/45)), alloc.height/2 - vet_v3[0] * escala_vel);
      for (j=0; j<contador; ++j)
  {
    cairo_line_to (cr, alloc.width * (1-(escala_t2/45)) + escala_t2 * vet_t[j], alloc.height/2 - vet_v3[j] * escala_vel);
  }
     cairo_stroke (cr);

    }

  return FALSE;
}


gboolean
graf_velocidade_posicao (GtkWidget *widget,
		       cairo_t *cr,
		       gpointer data)
{
  int j;
  GtkAllocation alloc;

  gtk_widget_get_allocation (draw_area4, &alloc);

  //Título
  cairo_move_to (cr, 100, 25);
  cairo_select_font_face (cr, "Calibri", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_font_size (cr, 18);
  cairo_set_source_rgb (cr, 0.263, 0.263, 0.263);
  cairo_show_text (cr, "Gráfico velocidade-posição - v(x)");
  cairo_stroke (cr);

  cairo_set_source_rgb (cr, 0, 0, 0);
  cairo_set_line_width (cr, 1.0);
  
  //Eixo horizontal
  cairo_move_to (cr, alloc.width/10, alloc.height/2);
  cairo_line_to (cr, alloc.width, alloc.height/2);
  cairo_move_to (cr, alloc.width, alloc.height/2);
  cairo_line_to (cr, (alloc.width)-5, (alloc.height/2)+5);
  cairo_move_to (cr, alloc.width, alloc.height/2);
  cairo_line_to (cr, (alloc.width)-5, (alloc.height/2)-5);
  
  cairo_stroke (cr);
  
  //Eixo vertical
  cairo_move_to (cr, 11*(alloc.width/20), alloc.height/10);
  cairo_line_to (cr, 11*(alloc.width/20), 9*(alloc.height/10));
  cairo_move_to (cr, 11*(alloc.width/20), alloc.height/10);
  cairo_line_to (cr, 11*(alloc.width/20)-5, (alloc.height/10)+5);
  cairo_move_to (cr, 11*(alloc.width/20), alloc.height/10);
  cairo_line_to (cr, 11*(alloc.width/20)+5, (alloc.height/10)+5);

  cairo_stroke (cr);
  
  //Identificação dos eixos
  cairo_set_font_size (cr, 13);
  cairo_select_font_face (cr, "Calibri", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
  cairo_move_to (cr, 11*(alloc.width/20)-10, (alloc.height/2)+12);
  cairo_show_text (cr, "0");
  cairo_move_to (cr, alloc.width-30, (alloc.height/2)+17);
  cairo_show_text (cr, "x(m)");
  cairo_move_to (cr, 11*(alloc.width/20)+5, (alloc.height/10)+5);
  cairo_show_text (cr, "v(m/s)");
  
  cairo_stroke (cr);

   //Gráfico

  cairo_set_line_width (cr, 3.0);
      
  //Massa1

  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (radio_button1)) == TRUE)
    {
      cairo_set_source_rgb (cr, r_m1, g_m1, b_m1);
      cairo_move_to (cr, 11*(alloc.width/20) + vet_x1[0] * escala_pos2, alloc.height/2 - vet_v1[0] * escala_vel);
      for (j=0; j<contador; ++j)
	{
	  cairo_line_to (cr, 11*(alloc.width/20) + vet_x1[j] * escala_pos2, alloc.height/2 - vet_v1[j] * escala_vel);
	}

      cairo_stroke (cr);
    }

  //Massa2

  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (radio_button2)) == TRUE)
    {
      cairo_set_source_rgb (cr, r_m2, g_m2, b_m2);
      cairo_move_to (cr, 11*(alloc.width/20) + vet_x2[0] * escala_pos2, alloc.height/2 - vet_v2[0] * escala_vel);
      for (j=0; j<contador; ++j)
	{
	  cairo_line_to (cr, 11*(alloc.width/20) + vet_x2[j] * escala_pos2, alloc.height/2 - vet_v2[j] * escala_vel);
	}

      cairo_stroke (cr);
    }

  //Massa3

  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (radio_button3)) == TRUE)
    {
      cairo_set_source_rgb (cr, r_m3, g_m3, b_m3);
      cairo_move_to (cr, 11*(alloc.width/20) + vet_x3[0] * escala_pos2, alloc.height/2 - vet_v3[0] * escala_vel);
      for (j=0; j<contador; ++j)
	{
	  cairo_line_to (cr, 11*(alloc.width/20) + vet_x3[j] * escala_pos2, alloc.height/2 - vet_v3[j] * escala_vel);
	}

      cairo_stroke (cr);
    }

  //Massas 1, 2 e 3

  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (radio_button4)) == TRUE)
    {
      cairo_set_source_rgb (cr, r_m1, g_m1, b_m1);
      cairo_move_to (cr, 11*(alloc.width/20) + vet_x1[0] * escala_pos2, alloc.height/2 - vet_v1[0] * escala_vel);
      for (j=0; j<contador; ++j)
	{
	  cairo_line_to (cr, 11*(alloc.width/20) + vet_x1[j] * escala_pos2, alloc.height/2 - vet_v1[j] * escala_vel);
	}

      cairo_stroke (cr);

      cairo_set_source_rgb (cr, r_m2, g_m2, b_m3);
      cairo_move_to (cr, 11*(alloc.width/20) + vet_x2[0] * escala_pos2, alloc.height/2 - vet_v2[0] * escala_vel);
      for (j=0; j<contador; ++j)
	{
	  cairo_line_to (cr, 11*(alloc.width/20) + vet_x2[j] * escala_pos2, alloc.height/2 - vet_v2[j] * escala_vel);
	}

      cairo_stroke (cr);

      cairo_set_source_rgb (cr, r_m3, g_m3, b_m3);
      cairo_move_to (cr, 11*(alloc.width/20) + vet_x3[0] * escala_pos2, alloc.height/2 - vet_v3[0] * escala_vel);
      for (j=0; j<contador; ++j)
	{
	  cairo_line_to (cr, 11*(alloc.width/20) + vet_x3[j] * escala_pos2, alloc.height/2 - vet_v3[j] * escala_vel);
	}

      cairo_stroke (cr);
    }


  return FALSE;
}


gboolean
grafico_window (GtkWidget *widget,
			 gpointer data)
{
  GtkWidget *window;
  GtkWidget *box1, *box2;
  GtkWidget *frame1, *frame2, *frame3;
  GtkWidget *scale1, *scale2, *scale3;
  GtkWidget *label1, *label2, *label3;
  GtkWidget *button;
  GtkAdjustment *adj_tempo, *adj_vel, *adj_pos;
  GtkStyleContext *context;
    
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);
  //gtk_window_move (GTK_WINDOW (window), 140, 350);
  gtk_window_set_title (GTK_WINDOW (window), "Gráfico velocidade-tempo");
  gtk_window_set_default_size (GTK_WINDOW (window), 1200, 300);
  gtk_window_set_resizable (GTK_WINDOW (window), FALSE);

  box1 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_container_add (GTK_CONTAINER (window), box1);

  frame1 = gtk_frame_new (NULL);
  gtk_widget_set_size_request (frame1, 450, 300);
  gtk_frame_set_shadow_type (GTK_FRAME (frame1), GTK_SHADOW_NONE);
  gtk_box_pack_start (GTK_BOX (box1), frame1, FALSE, FALSE, 25);

  frame2 = gtk_frame_new (NULL);
  gtk_widget_set_size_request (frame2, 450, 300);
  gtk_frame_set_shadow_type (GTK_FRAME (frame2), GTK_SHADOW_NONE);
  gtk_box_pack_start (GTK_BOX (box1), frame2, FALSE, FALSE, 0);
  
  frame3 = gtk_frame_new (NULL);
  gtk_widget_set_size_request (frame3, 250, 300);
  gtk_frame_set_shadow_type (GTK_FRAME (frame3), GTK_SHADOW_NONE);
  gtk_box_pack_end (GTK_BOX (box1), frame3, FALSE, FALSE, 20);

  box2 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add (GTK_CONTAINER (frame3), box2);
  
  //Radio buttons
  radio_button1 = gtk_radio_button_new_with_label (NULL, "Massa 1");
  gtk_box_pack_start (GTK_BOX (box2), radio_button1, FALSE, FALSE, 10);
  
  radio_button2 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio_button1), "Massa 2");
  gtk_box_pack_start (GTK_BOX (box2), radio_button2, FALSE, FALSE, 8);
  
  radio_button3 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio_button2), "Massa 3");
  gtk_box_pack_start (GTK_BOX (box2), radio_button3, FALSE, FALSE, 8);

  radio_button4 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio_button3), "Massas 1, 2 e 3");
  gtk_box_pack_start (GTK_BOX (box2), radio_button4, FALSE, FALSE, 8);

  radio_button5 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio_button4), "Ocultar gráficos");
  gtk_box_pack_end (GTK_BOX (box2), radio_button5, FALSE, FALSE, 20);

  //Regular escalas
  label1 = gtk_label_new ("Escala de tempo");
  gtk_widget_set_name (label1, "label_2");
  gtk_box_pack_start (GTK_BOX (box2), label1, FALSE, FALSE, 15);
  
  adj_tempo = gtk_adjustment_new (45, 45, 225, 1, 1, 0);
  scale1 = gtk_scale_new (GTK_ORIENTATION_HORIZONTAL, adj_tempo);
  gtk_scale_set_draw_value (GTK_SCALE (scale1), FALSE);
  gtk_box_pack_start (GTK_BOX (box2), scale1, FALSE, FALSE, 0);
  g_signal_connect (scale1, "value-changed", G_CALLBACK (escala_tempo2), NULL);

  label2 = gtk_label_new ("Escala de velocidade");
  gtk_widget_set_name (label2, "label_2");
  gtk_box_pack_start (GTK_BOX (box2), label2, FALSE, FALSE, 10);

  adj_vel = gtk_adjustment_new (40, 5, 80, 1, 1, 0);
  scale2 = gtk_scale_new (GTK_ORIENTATION_HORIZONTAL, adj_vel);
  gtk_scale_set_draw_value (GTK_SCALE (scale2), FALSE);
  gtk_box_pack_start (GTK_BOX (box2), scale2, FALSE, FALSE, 0);
  g_signal_connect (scale2, "value-changed", G_CALLBACK (escala_velocidade), NULL);

  label3 = gtk_label_new ("Escala de posição");
  gtk_widget_set_name (label3, "label_2");
  gtk_box_pack_start (GTK_BOX (box2), label3, FALSE, FALSE, 10);

  adj_pos = gtk_adjustment_new (40, 10, 80, 1, 1, 0);
  scale3 = gtk_scale_new (GTK_ORIENTATION_HORIZONTAL, adj_pos);
  gtk_scale_set_draw_value (GTK_SCALE (scale3), FALSE);
  gtk_box_pack_start (GTK_BOX (box2), scale3, FALSE, FALSE, 0);
  g_signal_connect (scale3, "value-changed", G_CALLBACK (escala_posicao2), NULL);

  //Botão para recomeçar sistema
  button = gtk_button_new_with_label ("Recomeçar");
  context = gtk_widget_get_style_context (button);
  gtk_style_context_add_class (context, "bt_style1");
  gtk_widget_set_size_request (button, 100, 50);
  gtk_box_pack_start (GTK_BOX (box2), button, FALSE, TRUE, 10);
  g_signal_connect (button, "clicked", G_CALLBACK (button_recomecar), NULL);
  
  //Drawing area
  draw_area3 = gtk_drawing_area_new();
  gtk_container_add (GTK_CONTAINER (frame1), draw_area3);
  g_signal_connect (draw_area3, "draw", G_CALLBACK (graf_velocidade_tempo), NULL);
  g_timeout_add (10, (GSourceFunc) time_h, draw_area3);

  draw_area4 = gtk_drawing_area_new();
  gtk_container_add (GTK_CONTAINER (frame2), draw_area4);
  g_signal_connect (draw_area4, "draw", G_CALLBACK (graf_velocidade_posicao), NULL);
  g_timeout_add (10, (GSourceFunc) time_h, draw_area4);
  
  gtk_widget_show_all (window);
   
  return FALSE;
}


gboolean
cor_massas (GtkWidget *widget,
	    gpointer data)
{
  srand(time(NULL));
  
  r_m1 = ((double) rand ()) / ((double) RAND_MAX);
  g_m1 = ((double) rand ()) / ((double) RAND_MAX);
  b_m1 = ((double) rand ()) / ((double) RAND_MAX);

  r_m2 = ((double) rand ()) / ((double) RAND_MAX);
  g_m2 = ((double) rand ()) / ((double) RAND_MAX);
  b_m2 = ((double) rand ()) / ((double) RAND_MAX);

  r_m3 = ((double) rand ()) / ((double) RAND_MAX);
  g_m3 = ((double) rand ()) / ((double) RAND_MAX);
  b_m3 = ((double) rand ()) / ((double) RAND_MAX);

  printf("%lf %lf %lf   %lf %lf %lf   %lf %lf %lf\n", r_m1, g_m1, b_m1, r_m2, g_m2, b_m2, r_m3, g_m3, b_m3);

  return FALSE;

}

gboolean
forma_massas (GtkWidget *widget,
	      gpointer data)
{
  if (forma==0)
    forma=1;

  else
    forma=0;
  
  return FALSE;
}

gboolean
func_about (GtkWidget *widget,
            gpointer p)
{
  GtkWidget *dialog;
  GdkPixbuf *image1;
  const gchar *authors[] = {"\n\nIvan Figueiredo, 93386\nTiago Miranda, 93416", NULL};
  gchar *info      = "Este programa tem como objetivo simular o sistema constituído por quatro molas iguais que se encontram ligadas a três massas também iguais.\n\nFoi desenvolvido no âmbito da disciplina de Programação do curso de Engenharia Física Tecnológica, no Instituto Superior Técnico.";



  image1 = gdk_pixbuf_new_from_file ("massa_mola.png", NULL);


  dialog = gtk_about_dialog_new();
  gtk_window_set_transient_for (GTK_WINDOW (dialog), GTK_WINDOW (p));
  gtk_window_set_default_size (GTK_WINDOW (dialog), 400, 150);
  gtk_about_dialog_set_program_name (GTK_ABOUT_DIALOG(dialog), "Sistema de 3 massas e 4 molas");
  gtk_about_dialog_set_authors (GTK_ABOUT_DIALOG(dialog), authors);
  gtk_about_dialog_set_comments (GTK_ABOUT_DIALOG(dialog), info);
  gtk_about_dialog_set_logo (GTK_ABOUT_DIALOG(dialog), image1);

  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy(dialog);

  return FALSE;
}


gboolean 
func_help_draw (GtkWidget *widget,
	            cairo_t *cr,
	            gpointer data)

{
  cairo_text_extents_t extents;
  GtkWidget *w;
  gint c, l;

  w = gtk_widget_get_toplevel (widget);
  gtk_window_get_size (GTK_WINDOW (w), &c, &l);
  cairo_select_font_face (cr, "Calibri", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_font_size (cr, 20);
  cairo_set_source_rgb (cr, 0.795, 0.169, 0.123);

  cairo_text_extents (cr, "Sistema de 3 massas e 4 molas", &extents);
  cairo_move_to (cr, c/2 - extents.width/2, 30);
  cairo_show_text (cr, "Sistema de 3 massas e 4 molas");
  cairo_stroke (cr);

  cairo_select_font_face (cr, "Calibri", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
  cairo_set_font_size (cr, 14);
  cairo_set_source_rgb (cr, 0, 0, 0);

  cairo_text_extents (cr, "Esta simulação representa um sistema formado por 3 massas iguais que se encontram", &extents);
  cairo_move_to (cr, c/2 - extents.width/2, 60);
  cairo_show_text (cr, "Esta simulação representa um sistema formado por 3 massas iguais que se encontram");

  cairo_text_extents (cr, "ligadas por 4 molas iguais. A massa 1 está sujeita ainda a uma força sinusoidal cuja", &extents);
  cairo_move_to (cr, c/2 - extents.width/2, 80);
  cairo_show_text (cr, "ligadas por 4 molas iguais. A massa 1 está sujeita ainda a uma força sinusoidal cuja");
  
  cairo_text_extents (cr, "intensidade varia com o tempo", &extents);
  cairo_move_to (cr, c/2 - extents.width/2, 100);
  cairo_show_text (cr, "intensidade varia com o tempo.");

  cairo_text_extents (cr, "O utilizadortem a possibilidade de alterar os parâmetros do sistema: a massa, a constante", &extents);
  cairo_move_to (cr, c/2 - extents.width/2, 130);
  cairo_show_text (cr, "O utilizador tem a possibilidade de alterar os parâmetros do sistema: a massa, a constante");

  cairo_text_extents (cr, "de elasticidade e o número de espiras das molas e a amplitude e a frequência angular da", &extents);
  cairo_move_to (cr, c/2 - extents.width/2, 150);
  cairo_show_text (cr, "de elasticidade e o número de espiras das molas e a amplitude e a frequência angular da");

  cairo_text_extents (cr, "força sinusoidal. É possível ainda definir o valor da posição e da velocidade inicial de cada uma", &extents);
  cairo_move_to (cr, c/2 - extents.width/2, 170);
  cairo_show_text (cr, "força sinusoidal. É possível ainda definir o valor da posição e da velocidade inicial de cada uma");

  cairo_text_extents (cr, "das massas. Para atualizar o sistema para as novas condições selecionadas, deverá carregar no", &extents);
  cairo_move_to (cr, c/2 - extents.width/2, 190);
  cairo_show_text (cr, "das massas. Para atualizar o sistema para as novas condições selecionadas, deverá carregar no");

  cairo_text_extents (cr, "botão INICIAR. Caso deseje utilizar as condições iniciais, carregue em RECOMEÇAR. Se pretender", &extents);
  cairo_move_to (cr, c/2 - extents.width/2, 210);
  cairo_show_text (cr, " botão INICIAR. Caso deseje utilizar as condições iniciais, carregue em RECOMEÇAR. Se pretender");

  cairo_text_extents (cr, "parar a simulação e os gráficos, carregue em PARAR. Para continuar, pressione CONTINUAR.", &extents);
  cairo_move_to (cr, c/2 - extents.width/2, 230);
  cairo_show_text (cr, "parar a simulação e os gráficos, carregue em PARAR. Para continuar, pressione CONTINUAR.");
 //
  cairo_text_extents (cr, "É possível observar, em tempo real e para cada uma das massas, o gráfico posição tempo - ", &extents);
  cairo_move_to (cr, c/2 - extents.width/2, 260);
  cairo_show_text (cr, "É possível observar, em tempo real e para cada uma das massas, o gráfico posição tempo - ");

  cairo_text_extents (cr, "na janela normal - e os gráficos velocidade-tempo e velocidade-posição - acessíveis através", &extents);
  cairo_move_to (cr, c/2 - extents.width/2, 280);
  cairo_show_text (cr, "na janela normal - e os gráficos velocidade-tempo e velocidade-posição - acessíveis através");

  cairo_text_extents (cr, "do botão 'Gráficos v(t) e v(x)'. Para cada um desses gráficos, é possível alterar o valor do", &extents);
  cairo_move_to (cr, c/2 - extents.width/2, 300);
  cairo_show_text (cr, "do botão 'Gráficos v(t) e v(x)'. Para cada um desses gráficos, é possível alterar o valor do");

  cairo_text_extents (cr, "deslocamento de cada massa em relação à posição de equilíbio, o valor da velocidade de cada", &extents);
  cairo_move_to (cr, c/2 - extents.width/2, 320);
  cairo_show_text (cr, "deslocamento de cada massa em relação à posição de equilíbio, o valor da velocidade de cada");
//
  cairo_text_extents (cr, "massa e a intensidade da força sinusoidal. Se pretender observar, em tempo real, os vetores", &extents);
  cairo_move_to (cr, c/2 - extents.width/2, 340);
  cairo_show_text (cr, "massa e a intensidade da força sinusoidal. Se pretender observar, em tempo real, os vetores");

  cairo_text_extents (cr, "representativos da velocidade, posição e força sinusoidal, ative os botões", &extents);
  cairo_move_to (cr, c/2 - extents.width/2, 360);
  cairo_show_text (cr, "representativos da velocidade, posição e força sinusoidal, ative os botões");

  cairo_text_extents (cr, "'Vetores v', 'Vetores x' e 'Vetor F'.", &extents);
  cairo_move_to (cr, c/2 - extents.width/2, 380);
  cairo_show_text (cr, "'Vetores v', 'Vetores x' e 'Vetor F'.");

  cairo_text_extents (cr, "Para sair, carregue na bola vermelha no canto superior direito ou no item 'Sair' do menu.", &extents);
  cairo_move_to (cr, c/2 - extents.width/2, 410);
  cairo_show_text (cr, "Para sair, carregue na bola vermelha no canto superior direito ou no item 'Sair' do menu.");

  cairo_stroke (cr);
  

	return FALSE;
}


gboolean
func_help (GtkWidget *widget,
	     gpointer data)
{
  GtkWidget *v_window;
  GtkWidget *vbox, *box1, *box2;
  GtkWidget *draw;
  GtkWidget *frame;
  GtkWidget *imagem1, *imagem2;
  
  v_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position (GTK_WINDOW (v_window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size (GTK_WINDOW (v_window), 800, 630);
  gtk_window_set_title (GTK_WINDOW (v_window), "Funcionamento do programa");
  gtk_window_set_resizable (GTK_WINDOW (v_window), FALSE);

  vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add (GTK_CONTAINER (v_window), vbox);

  box1 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_set_size_request (box1, 800, 50);
  gtk_box_pack_start (GTK_BOX (vbox), box1, FALSE, FALSE, 0);

  frame = gtk_frame_new (NULL);
  gtk_widget_set_size_request (frame, 800, 430);
  gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);
  gtk_box_pack_start (GTK_BOX (vbox), frame, FALSE, FALSE, 0);

  draw = gtk_drawing_area_new ();
  gtk_container_add (GTK_CONTAINER (frame), draw);
  g_signal_connect (draw, "draw", G_CALLBACK (func_help_draw), NULL);
  g_timeout_add (10, (GSourceFunc) draw_function, draw_area1);

  box2 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_set_size_request (box2, 800, 150);
  gtk_box_pack_start (GTK_BOX (vbox), box2, FALSE, FALSE, 0);

  imagem1 = gtk_image_new_from_file ("imagem_1.png");
  imagem2 = gtk_image_new_from_file ("ist.jpg");

  gtk_box_pack_start (GTK_BOX (box1), imagem2, FALSE, FALSE, 10);
  gtk_box_pack_start (GTK_BOX (box2), imagem1, FALSE, FALSE, 150);


  gtk_widget_show_all (v_window);

  return FALSE;
}
    

int main (int argc, char **argv)
{
  GtkWidget *window;
  GtkWidget *vbox, *box_menu, *box0, *box1, *box2, *box3, *box4, *box5, *box6, *box7, *box8, *box9, *box10, *box11, *box12, *box13, *box14, *box15, *box16, *box17, *box18, *box19, *box20, *box21, *box22, *box23, *box26, *box27, *box28, *box29, *box30;
  GtkWidget *frame1, *frame2, *frame3, *frame4, *frame5, *frame6, *frame7, *frame8, *frame9;
  GtkWidget *label1, *label2, *label3, *label4, *label5, *label6, *label7, *label8, *label9, *label10, *label11, *label12,*label13, *label14, *label15, *label16, *label18, *label19, *label20, *label21, *label22, *label23;
  GtkWidget *image1, *image2, *image3;
  GtkWidget *button3, *button4, *button5, *button6;
  GtkWidget *menu_bar, *menu, *item1, *item2, *item3, *item4, *sep1, *sep2, *sep3;
  GtkWidget *scale1, *scale2, *scale3;
  
  GtkAdjustment *adj_xx, *adj_yy, *adj_esp;
  GtkAllocation alloc1;
  GtkStyleContext *context;

  gtk_init (&argc, &argv);
  provider_create (cssData);

  comp_string = strlen (pos_1);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);
  gtk_window_set_title (GTK_WINDOW (window), "Sistema de 3 massas e 4 molas");
  gtk_window_set_default_size (GTK_WINDOW (window), 1100, 700);
  gtk_widget_set_size_request (window, 1100, 700);
  //gtk_window_set_resizable (GTK_WINDOW (window), FALSE);
  
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
  
  vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add (GTK_CONTAINER (window), vbox);

  box_menu = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_size_request (box_menu, 1100, 30);
  gtk_box_pack_start (GTK_BOX (vbox), box_menu, FALSE, FALSE, 0);

  box0 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_set_size_request (box0, 1100, 670);
  gtk_box_pack_start (GTK_BOX (vbox), box0, FALSE, FALSE, 0);
  
  box1 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_size_request (box1, 800, 670);
  gtk_box_pack_start (GTK_BOX (box0), box1, TRUE, TRUE, 0);

  //Criar menu
  menu_bar = gtk_menu_bar_new ();
  gtk_box_pack_start (GTK_BOX (box_menu), menu_bar, FALSE, FALSE, 0);
  
  item1 = gtk_menu_item_new_with_label ("Menu");
  gtk_widget_set_size_request (item1, 100, 30);
  gtk_menu_shell_append (GTK_MENU_SHELL (menu_bar), item1);
  
  menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (item1), menu);

  item2 = gtk_menu_item_new_with_label ("Acerca");
  gtk_widget_set_size_request (item2, 100, 20);
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), item2);
  g_signal_connect (item2, "activate", G_CALLBACK (func_about), window);

  sep1 = gtk_separator_menu_item_new();
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), sep1);

  item4 = gtk_menu_item_new_with_label ("Ajuda");
  gtk_widget_set_size_request (item4, 100, 20);
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), item4);
  g_signal_connect (item4, "activate", G_CALLBACK (func_help), window);

  sep2 = gtk_separator_menu_item_new();
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), sep2);
  
  item3 = gtk_menu_item_new_with_label ("Sair");
  gtk_widget_set_size_request (item3, 100, 20);
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), item3);
  g_signal_connect (item3, "activate", G_CALLBACK (gtk_main_quit), NULL);

  sep3 = gtk_separator_menu_item_new();
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), sep3);
  

  //Frame 
  frame1 = gtk_frame_new (NULL); 
  gtk_widget_set_size_request(frame1, 800, 300);
  gtk_box_pack_start (GTK_BOX (box1), frame1, FALSE, FALSE, 0);


//Área do gráfico posição-tempo
  frame3 = gtk_frame_new (NULL);
  gtk_widget_set_size_request (frame3, 800, 370);
  gtk_box_pack_start (GTK_BOX (box1), frame3, FALSE, FALSE, 0);
  
  box3 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_container_add (GTK_CONTAINER (frame3), box3);

  frame4 = gtk_frame_new (NULL);
  gtk_widget_set_size_request (frame4, 450, 370);
  gtk_frame_set_shadow_type (GTK_FRAME (frame4), GTK_SHADOW_NONE);
  gtk_box_pack_start (GTK_BOX (box3), frame4, FALSE, TRUE, 50);

  frame5 = gtk_frame_new (NULL);
  gtk_widget_set_size_request (frame5, 250, 370);
  gtk_frame_set_shadow_type (GTK_FRAME (frame5), GTK_SHADOW_NONE);
  gtk_box_pack_end (GTK_BOX (box3), frame5, TRUE, FALSE, 25);

  box4 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add (GTK_CONTAINER (frame5), box4);
  
  //Drawing area - sistema massa mola
  draw_area1 = gtk_drawing_area_new();
  gtk_container_add (GTK_CONTAINER (frame1), draw_area1);

  g_signal_connect (draw_area1, "draw", G_CALLBACK (draw_function), NULL);
  g_timeout_add (10, (GSourceFunc) draw_function, draw_area1);

  
  //Drawing area - grafico posição tempo	     
  draw_area2 = gtk_drawing_area_new();
  gtk_container_add (GTK_CONTAINER (frame4), draw_area2);

  g_signal_connect (draw_area2, "draw", G_CALLBACK (graf_posicao_tempo), NULL);
  g_timeout_add (10, (GSourceFunc) time_h, draw_area2);

  //Comandos do programa
  box5 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_size_request (box5, 300, 670);
  gtk_box_pack_end (GTK_BOX (box0), box5, FALSE, FALSE, 0);

  context = gtk_widget_get_style_context (box5);
  gtk_style_context_add_class (context, "box_style");
  
  frame6 = gtk_frame_new (NULL);
  gtk_widget_set_size_request (frame6, 300, 300);
  gtk_box_pack_start (GTK_BOX (box5), frame6, FALSE, FALSE, 0);

  box6 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add (GTK_CONTAINER (frame6), box6);
  
  label1 = gtk_label_new ("Parâmetros do sistema");
  gtk_widget_set_name (label1, "label_1");
  gtk_box_pack_start (GTK_BOX (box6), label1, FALSE, FALSE, 10);

  //Parâmetros do sistema
  //Massa
  box7 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_set_size_request (box7, 300, 50);
  gtk_box_pack_start (GTK_BOX (box6), box7, FALSE, FALSE, 0);

  label2 = gtk_label_new ("Massa");
  gtk_widget_set_name (label2, "label_2");
  gtk_box_pack_start (GTK_BOX (box7), label2, FALSE, FALSE, 10 );

  spin_m = create_spin_button_m ();
  gtk_box_pack_end (GTK_BOX (box7), spin_m, FALSE, FALSE, 10);

  
  //Constante de elasticidade
  box8 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_set_size_request (box8, 300, 50);
  gtk_box_pack_start (GTK_BOX (box6), box8, FALSE, FALSE, 0);

  label3 = gtk_label_new ("Constante\nde elasticidade\n");
  gtk_widget_set_name (label3, "label_2");
  gtk_box_pack_start (GTK_BOX (box8), label3, FALSE, FALSE, 10 );
  
  spin_k = create_spin_button_k ();
  gtk_box_pack_end (GTK_BOX (box8), spin_k, FALSE, FALSE, 10);

  
  //Amplitude da força
  box9 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_set_size_request (box9, 300, 50);
  gtk_box_pack_start (GTK_BOX (box6), box9, FALSE, FALSE, 0);

  label4 = gtk_label_new ("Amplitude da\nforça");
  gtk_widget_set_name (label4, "label_2");
  gtk_box_pack_start (GTK_BOX (box9), label4, FALSE, FALSE, 10 );
  
  spin_a = create_spin_button_a ();
  gtk_box_pack_end (GTK_BOX (box9), spin_a, FALSE, FALSE, 10);

  
  //Pulsação da força 
  box10 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_set_size_request (box10, 300, 50);
  gtk_box_pack_start (GTK_BOX (box6), box10, FALSE, FALSE, 0);

  label5 = gtk_label_new ("Frequência\nangular da\nforça");
  gtk_widget_set_name (label5, "label_2");
  gtk_box_pack_start (GTK_BOX (box10), label5, FALSE, FALSE, 10);
  
  spin_p = create_spin_button_p ();
  gtk_box_pack_end (GTK_BOX (box10), spin_p, FALSE, FALSE, 10);

  
  //Número de espiras de cada mola
  box23 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_set_size_request (box23, 300, 50);
  gtk_box_pack_start (GTK_BOX (box6), box23, FALSE, FALSE, 0);

  label18 = gtk_label_new ("Número de\nespiras");
  gtk_widget_set_name (label18, "label_2");
  gtk_box_pack_start (GTK_BOX (box23), label18, FALSE, FALSE, 10);

  adj_esp = gtk_adjustment_new (11, 9, 25, 2, 1, 0);
  scale1 = gtk_scale_new (GTK_ORIENTATION_HORIZONTAL, adj_esp);
  gtk_scale_set_draw_value (GTK_SCALE (scale1), FALSE);
  g_object_set(scale1, "width-request", 150, NULL); 
  gtk_box_pack_end (GTK_BOX (box23), scale1, FALSE, FALSE, 10);
  g_signal_connect (scale1, "value-changed", G_CALLBACK (espiras_mola), NULL);

  
  //Massa1
  frame7 = gtk_frame_new (NULL);
  gtk_widget_set_size_request (frame7, 300, 90);
  gtk_box_pack_start (GTK_BOX (box5), frame7, FALSE, FALSE, 0);
  
  box11 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add (GTK_CONTAINER (frame7), box11);
  
  label6 = gtk_label_new ("Massa 1");
  gtk_widget_set_name (label6, "label_1");
  gtk_box_pack_start (GTK_BOX (box11), label6, FALSE, FALSE, 3);

  box12 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_box_pack_start (GTK_BOX (box11), box12, FALSE, FALSE, 0);

  label7 = gtk_label_new ("Posição inicial");
  gtk_widget_set_name (label7, "label_2");
  gtk_box_pack_start (GTK_BOX (box12), label7, FALSE, FALSE, 10);

  label8 = gtk_label_new ("Velocidade inicial");
  gtk_widget_set_name (label8, "label_2");
  gtk_box_pack_end (GTK_BOX (box12), label8, FALSE, FALSE, 10);

  box13 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_box_pack_start (GTK_BOX (box11), box13, FALSE, FALSE, 0);

  spin_pos_1 = create_spin_button_pos_1 ();
  gtk_box_pack_start (GTK_BOX (box13), spin_pos_1, FALSE, FALSE, 10);

  spin_vel_1 = create_spin_button_vel_1 ();
  gtk_box_pack_end (GTK_BOX (box13), spin_vel_1, FALSE, FALSE, 10);

  
  //Massa2
  frame8 = gtk_frame_new (NULL);
  gtk_widget_set_size_request (frame8, 300, 90);
  gtk_box_pack_start (GTK_BOX (box5), frame8, FALSE, FALSE, 0);

  box14 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add (GTK_CONTAINER (frame8), box14);
  
  label9 = gtk_label_new ("Massa 2");
  gtk_widget_set_name (label9, "label_1");
  gtk_box_pack_start (GTK_BOX (box14), label9, FALSE, FALSE, 3);

  box15 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_box_pack_start (GTK_BOX (box14), box15, FALSE, FALSE, 0);

  label10 = gtk_label_new ("Posição inicial");
  gtk_widget_set_name (label10, "label_2");
  gtk_box_pack_start (GTK_BOX (box15), label10, FALSE, FALSE, 10);

  label11 = gtk_label_new ("Velocidade inicial");
  gtk_widget_set_name (label11, "label_2");
  gtk_box_pack_end (GTK_BOX (box15), label11, FALSE, FALSE, 10);

  box16 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_box_pack_start (GTK_BOX (box14), box16, FALSE, FALSE, 0);

  spin_pos_2 = create_spin_button_pos_2 ();
  gtk_box_pack_start (GTK_BOX (box16), spin_pos_2, FALSE, FALSE, 10);

  spin_vel_2 = create_spin_button_vel_2 ();
  gtk_box_pack_end (GTK_BOX (box16), spin_vel_2, FALSE, FALSE, 10);

  
  //Massa3
  frame9 = gtk_frame_new (NULL);
  gtk_widget_set_size_request (frame9, 300, 90);
  gtk_box_pack_start (GTK_BOX (box5), frame9, FALSE, FALSE, 0);

  box17 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add (GTK_CONTAINER (frame9), box17);
  
  label12 = gtk_label_new ("Massa 3");
  gtk_widget_set_name (label12, "label_1");
  gtk_box_pack_start (GTK_BOX (box17), label12, FALSE, FALSE, 3);

  box18 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_box_pack_start (GTK_BOX (box17), box18, FALSE, FALSE, 0);

  label13 = gtk_label_new ("Posição inicial");
  gtk_widget_set_name (label13, "label_2");
  gtk_box_pack_start (GTK_BOX (box18), label13, FALSE, FALSE, 10);

  label14 = gtk_label_new ("Velocidade inicial");
  gtk_widget_set_name (label14, "label_2");
  gtk_box_pack_end (GTK_BOX (box18), label14, FALSE, FALSE, 10);

  box19 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_box_pack_start (GTK_BOX (box17), box19, FALSE, FALSE, 0);

  spin_pos_3 = create_spin_button_pos_3 ();
  gtk_box_pack_start (GTK_BOX (box19), spin_pos_3, FALSE, FALSE, 10);

  spin_vel_3 = create_spin_button_vel_3 ();
  gtk_box_pack_end (GTK_BOX (box19), spin_vel_3, FALSE, FALSE, 10);

  //Botões
  box20 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_set_size_request (box20, 300, 50);
  gtk_box_pack_start (GTK_BOX (box5), box20, FALSE, FALSE, 0);

  b_parar = gtk_button_new_with_label ("Parar");
  context = gtk_widget_get_style_context (b_parar);
  gtk_style_context_add_class (context, "bt_style1");
  gtk_widget_set_size_request (b_parar, 100, 50);
  gtk_box_pack_start (GTK_BOX (box20), b_parar, FALSE, TRUE, 2);
  g_signal_connect (b_parar, "clicked", G_CALLBACK (button_parar), NULL);
  
  b_iniciar = gtk_button_new_with_label ("Iniciar");
  context = gtk_widget_get_style_context (b_iniciar);
  gtk_style_context_add_class (context, "bt_style1");
  gtk_widget_set_size_request (b_iniciar, 100, 50);
  gtk_box_pack_start (GTK_BOX (box20), b_iniciar, FALSE, TRUE, 0);
  g_signal_connect (b_iniciar, "clicked", G_CALLBACK (button_iniciar), NULL);
  
  b_recomecar = gtk_button_new_with_label ("Recomeçar");
  context = gtk_widget_get_style_context (b_recomecar);
  gtk_style_context_add_class (context, "bt_style1");
  gtk_widget_set_size_request (b_recomecar, 100, 50);
  gtk_box_pack_start (GTK_BOX (box20), b_recomecar, FALSE, TRUE, 0);
  g_signal_connect (b_recomecar, "clicked", G_CALLBACK (button_recomecar), NULL);

  //Cor das massas e botão informativo

  box21 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_set_size_request (box21, 300, 50);
  gtk_box_pack_start (GTK_BOX (box5), box21, FALSE, FALSE, 0);

  button3 = gtk_button_new_with_label ("Cor");
  context = gtk_widget_get_style_context (button3);
  gtk_style_context_add_class (context, "bt_style2");
  gtk_widget_set_size_request (button3, 100, 50);
  g_signal_connect (button3, "clicked", G_CALLBACK (cor_massas), NULL);
  gtk_box_pack_start (GTK_BOX (box21), button3, FALSE, TRUE, 2);

  button4 = gtk_button_new_with_label ("Forma");
  context = gtk_widget_get_style_context (button4);
  gtk_style_context_add_class (context, "bt_style2");
  gtk_widget_set_size_request (button4, 100, 50);
  g_signal_connect (button4, "clicked", G_CALLBACK (forma_massas), NULL);
  gtk_box_pack_start (GTK_BOX (box21), button4, FALSE, TRUE, 0);

  image1 = gtk_image_new_from_icon_name ("dialog-question", GTK_ICON_SIZE_LARGE_TOOLBAR);
  button5 = gtk_button_new ();
  gtk_button_set_image (GTK_BUTTON (button5), image1);
  gtk_widget_set_size_request (button5, 100, 50);
  gtk_box_pack_start (GTK_BOX (box21), button5, FALSE, TRUE, 0);
  g_signal_connect (button5, "clicked", G_CALLBACK (func_help), NULL);

  ///Swich buttons na simulaçao
  box26 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_set_size_request (box26, 250, 15);
  gtk_box_pack_start (GTK_BOX (box4), box26, FALSE, FALSE, 2);

  label19 = gtk_label_new ("Valores de x, v, F");
  gtk_widget_set_name (label19, "label_2");
  gtk_box_pack_start (GTK_BOX (box26), label19, FALSE, FALSE, 0);

  switch1 = gtk_switch_new ();
  gtk_widget_set_size_request (switch1, 80, 15);
  gtk_switch_set_active (GTK_SWITCH (switch1), TRUE);
  gtk_box_pack_end (GTK_BOX (box26), switch1, FALSE, FALSE, 0);
//
  box27 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_set_size_request (box27, 250, 15);
  gtk_box_pack_start (GTK_BOX (box4), box27, FALSE, FALSE, 2);

  label20 = gtk_label_new ("Vetores v");
  gtk_widget_set_name (label20, "label_2");
  gtk_box_pack_start (GTK_BOX (box27), label20, FALSE, FALSE, 0);

  switch2 = gtk_switch_new ();
  gtk_widget_set_size_request (switch2, 80, 15);
  gtk_box_pack_end (GTK_BOX (box27), switch2, FALSE, FALSE, 0);
//
  box30 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_set_size_request (box30, 250, 15);
  gtk_box_pack_start (GTK_BOX (box4), box30, FALSE, FALSE, 2);

  label23 = gtk_label_new ("Vetores x");
  gtk_widget_set_name (label23, "label_2");
  gtk_box_pack_start (GTK_BOX (box30), label23, FALSE, FALSE, 0);

  switch5 = gtk_switch_new ();
  gtk_widget_set_size_request (switch5, 80, 15);
  gtk_switch_set_active (GTK_SWITCH (switch5), TRUE);
  gtk_box_pack_end (GTK_BOX (box30), switch5, FALSE, FALSE, 0);
//
  box28 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_set_size_request (box28, 250, 15);
  gtk_box_pack_start (GTK_BOX (box4), box28, FALSE, FALSE, 2);

  label21 = gtk_label_new ("Vetor F");
  gtk_widget_set_name (label21, "label_2");
  gtk_box_pack_start (GTK_BOX (box28), label21, FALSE, FALSE, 0);

  switch3 = gtk_switch_new ();
  gtk_widget_set_size_request (switch3, 80, 15);
  gtk_box_pack_end (GTK_BOX (box28), switch3, FALSE, FALSE, 0);
//
  box29 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_set_size_request (box29, 250, 15);
  gtk_box_pack_start (GTK_BOX (box4), box29, FALSE, FALSE, 2);

  label22 = gtk_label_new ("Escala horizontal");
  gtk_widget_set_name (label22, "label_2");
  gtk_box_pack_start (GTK_BOX (box29), label22, FALSE, FALSE, 0);

  switch4 = gtk_switch_new ();
  gtk_widget_set_size_request (switch4, 80, 15);
  gtk_switch_set_active (GTK_SWITCH (switch4), TRUE);
  gtk_box_pack_end (GTK_BOX (box29), switch4, FALSE, FALSE, 0);

  //Scale para regular a escala do tempo
  label15 = gtk_label_new ("Escala de tempo");
  gtk_widget_set_name (label15, "label_2");
  gtk_box_pack_start (GTK_BOX (box4), label15, FALSE, FALSE, 10);

  adj_xx = gtk_adjustment_new (45, 45, 225, 1, 1, 0);
  scale2 = gtk_scale_new (GTK_ORIENTATION_HORIZONTAL, adj_xx);
  gtk_scale_set_draw_value (GTK_SCALE (scale2), FALSE);
  gtk_box_pack_start (GTK_BOX (box4), scale2, FALSE, FALSE, 0);
  g_signal_connect (scale2, "value-changed", G_CALLBACK (escala_tempo1), NULL);

  label16 = gtk_label_new ("Escala de posição");
  gtk_widget_set_name (label16, "label_2");
  gtk_box_pack_start (GTK_BOX (box4), label16, FALSE, FALSE, 5);

  adj_yy = gtk_adjustment_new (40, 10, 70, 1, 1, 0);
  scale3 = gtk_scale_new (GTK_ORIENTATION_HORIZONTAL, adj_yy);
  gtk_scale_set_draw_value (GTK_SCALE (scale3), FALSE);
  gtk_box_pack_start (GTK_BOX (box4), scale3, FALSE, FALSE, 0);
  g_signal_connect (scale3, "value-changed", G_CALLBACK (escala_posicao1), NULL);
  
   //Radio buttons
  radio_button6 = gtk_radio_button_new_with_label (NULL, "Visualizar gráfico");
  gtk_box_pack_start (GTK_BOX (box4), radio_button6, FALSE, FALSE, 15);
  
  radio_button7 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio_button6), "Ocultar gráfico");
  gtk_box_pack_start (GTK_BOX (box4), radio_button7, FALSE, FALSE, 0);

  button6 = gtk_button_new_with_label ("Gráficos v(t) e v(x)");
  context = gtk_widget_get_style_context (button6);
  gtk_style_context_add_class (context, "bt_style3");
  gtk_widget_set_size_request (button6, 100, 35);
  gtk_box_pack_start (GTK_BOX (box4), button6, FALSE, FALSE, 10);
  g_signal_connect (button6, "clicked", G_CALLBACK (grafico_window), NULL);

  get_values ();
  
  gtk_widget_show_all (window);
  gtk_main();

  return 0;
}