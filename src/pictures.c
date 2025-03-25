#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "pictures.h"
#include "filename.h"
#include "lut.h"
#include "pixels.h"

const int MAX_BYTE = 255;

/*  @requires filename the name of an existing .pgm or .ppm file;
    @assigns nothing;
    @ensures returns a picture filled with the filename data. On failure it returns an error message; */
picture read_picture(char* filename){
    // Ouverture du fichier en lecture.
    FILE* fd = canopen(filename);

    // Affichage d'un message d'erreur si le fichier n'est pas trouvé.
    if(fd==NULL) exit(-1);

    picture p;

    char buffer[128];

    // Extraction de la première ligne de notre fichier dans le buffer.
    fgets(buffer,128,fd);

    // Extraction de l'extention de filename.
    char* extention = ext_from_path(filename);

    // Vérification de la conformité du fichier.
    if((strcmp(extention, "ppm") && buffer[1]!='5')||(strcmp(extention, "pgm") && buffer[1]!='6')){
        fprintf(stderr,"Error : %s content does not match the expected format\n",filename);
        exit(-1);
    }

    // Initialisation du nombre de canaux de l’image.
    int magic_number;
    sscanf(buffer,"P%d",&(magic_number));
    if(magic_number==5) p.channel = 1;
    else p.channel = 3;

    // Extraction de la ligne suivante.
    fgets(buffer,128,fd);

    // Extraction de la deuxième ligne non commenté du fichier dans le buffer.
    while(buffer[0]=='#')
        // Extraction d'une ligne du fichier.
        fgets(buffer,128,fd);
    
    // Extraction de la largeur et la hauteur de l'image dans notre buffer et assignation de ses valeurs.
    sscanf(buffer,"%d %d",&(p.width),&(p.height));

    // Extraction de la ligne suivante.
    fgets(buffer,128,fd);

    // Extraction de la troisième ligne non commenté du fichier dans le buffer.
    while(buffer[0]=='#')
        // Extraction d'une ligne du fichier.
        fgets(buffer,128,fd);

    int max;

    // Extraction de la valeur maximal d'un octet du fichier.
    sscanf(buffer,"%d",&(max));

    long position = ftell(fd);

    // Extraction de la ligne suivante.
    fgets(buffer,128,fd);

    // Extraction de la quatrième ligne non commenté du fichier dans le buffer.
    while(buffer[0]=='#'){
        position = ftell(fd);
        // Extraction d'une ligne du fichier.
        fgets(buffer,128,fd);
    }

    p.tbl = malloc(byte_number(p)*sizeof(byte));

    // Placement du curseur au début de la séquence d'octect.
    fseek(fd, position, SEEK_SET);

    // Remplissage du tableau contenant les octets de l'image.
    fread(p.tbl,sizeof(byte),byte_number(p),fd);

    // Normalisation des valeurs des composantes des pixels (par 255/MAX)
     if (MAX_BYTE != 255) {
        for (int i = 0; i<byte_number(p); i++) {
            p.tbl[i] = (p.tbl[i] * 255) / MAX_BYTE;
        }
    }

    // Fermeture du fichier.
    fclose(fd);

    return p;
}

/*  @requires filename the name of a .pgm or .ppm file. p a valid picture structure;
    @assigns the content of filename;
    @ensures write the content of p in filename. On success returns null. On failure returns -1; */
int write_picture(picture p, char * filename){

    // Vérification de l'existance du fichier.
    FILE* fd = canopen(filename);
    // On retourne -1 en cas d'erreur.
    if(fd==NULL) return -1;

    // Extraction du magic number.
    int magic_number;
    if(p.channel==1) magic_number = 5;
    else magic_number = 6;

    // Extraction de l'extention du fichier.
    char* extention = ext_from_path(filename);

    // Vérification pour savoir si le type de l’image correspond au type de fichier demandé.
    if((p.channel==3 && strcmp(extention, "ppm")) || (p.channel==1 && strcmp(extention, "pgm"))){
        fprintf(stderr,"Error : the image type does not match %s type\n",filename);
        return -1;
    }

    // Ouverture du fichier en écriture.
    fd = canwrite(filename);

    // On retourne -1 en cas d'erreur.
    if(fd==NULL) return -1;

    // Écriture de l'image p dans le fichier.
    fprintf(fd,"P%d\n%d %d\n%d\n",magic_number,p.width,p.height,MAX_BYTE);
    fwrite(p.tbl,sizeof(byte),byte_number(p),fd);

    // Fermeture du fichier.
    fclose(fd);

    return 0;
}

/*  @requires filename the name of a file;
    @assigns nothing;
    @ensures checks if filename can be open reading. On success returns a file descriptor for reading. On failure returns NULL; */
FILE* canopen(char* filename){
   FILE* fd = fopen(filename,"r");
   // Si fd est NULL on renvoie un message d'erreur.
   if(fd==NULL){
        fprintf(stderr,"Error : %s not found\n",filename);
        return NULL;
   }
   return fd;
}

/*  @requires filename the name of a file;
    @assigns nothing;
    @ensures checks if filename can be open writing. On success returns a file descriptor for writing. On failure returns NULL; */
FILE* canwrite(char* filename){
   // On vérifie d'abord si fd est ouvrable en lecture seul.
   FILE* fd = canopen(filename);
   if(fd==NULL) return NULL;
   // On vérifie maintenant si le fichier est ouvrable en écriture.
   fd = fopen(filename,"w");
   if(fd==NULL){
        fprintf(stderr,"Error : %s can't be writing\n",filename); 
        return NULL;
   }
   return fd;
}

/*  @requires three unsigned integers representing respectively the width, height and number channels of a picture and the maximum of a byte;
    @assigns nothing;
    @ensures returns a picture initialized with the parameters provided to the function; */
picture create_picture(int width, int height, int channels, byte max){
    picture p;
    p.width = width;
    p.height = height;
    p.channel = channels;
    p.tbl = malloc(byte_number(p)*sizeof(byte));
    // On vérifie que le tableau existe.
    if (p.tbl){
        // Les pixels prennent la valeur max.
        for (int i = 0; i<byte_number(p); ++i) {
            p.tbl[i] = max;
        }
    }
    return p;
}

/*  @requires p a pointer to a picture;
    @assigns p;
    @ensures removes data from p; */
void clean_picture(picture* p){
    // On vérifie si le pointeur existe.
    if(p!=NULL){
        // On vérifie si le pointeur vers le tableau existe.
        if(p->tbl!=NULL){
            free(p->tbl);
            p->tbl=NULL;
        }
        // On initialise les valeurs.
        p->height = 0;
        p->width = 0;
        p->channel = 0;
        p = NULL;
    }
}

/*  @requires p a picture;
    @assigns nothing;
    @ensures returns a copy of the picture p; */
picture copy_picture(picture p){
    picture p_copy;
    // On initialise la taille et les channels de la copie.
    p_copy.width=p.width;
    p_copy.height=p.height;
    p_copy.channel=p.channel;
    // On vérifie que le pointeur vers le tableau n'exisite pas.
    if(p.tbl==NULL){
        return p_copy;
    }
    p_copy.tbl = malloc(byte_number(p_copy)*sizeof(byte));
    for(int i=0; i<byte_number(p_copy); i++){
        // On initialise la valeurs des pixels de la copie avec celle de base.
        p_copy.tbl[i]=p.tbl[i];
    }
    return p_copy;
}

/*  @requires p a picture;
    @assigns nothing;
    @ensures returns 1 if p is empty 0 otherwise; */
int is_empty_picture(picture p){
    if (p.width<=0 || p.height<=0 || p.channel<=0 || p.tbl==NULL) return 1;
    return 0;
}

/*  @requires p a picture;
    @assigns nothing;
    @ensures returns 1 if p is a gray picture 0 otherwise; */
int is_gray_picture(picture p){
    if (is_empty_picture(p) || p.channel!=1) return 0;
    return 1;
}

/*  @requires p a picture;
    @assigns nothing;
    @ensures returns 1 if p is a color picture 0 otherwise; */
int is_color_picture(picture p){
    if (is_empty_picture(p) || p.channel!=3) return 0;
    return 1;
}

/*  @requires p a picture;
    @assigns nothing;
    @ensures print the string: “(<width> x <height> x <channels>)” where <width>, <height> and <channels> are to be replaced by their respective values in p.; */
void info_picture(picture p){
    printf("%d x %d x %d\n",p.width,p.height,p.channel);
}

/*  @requires p a picture;
    @assigns nothing;
    @ensures on sucess it returns p converted into colored format. On failure it returns an empty picture; */
picture convert_to_color_picture(picture p){
    picture p_copy = copy_picture(p);
    // Si la picture est déjà en couleur on renvoie la copie.
    if(is_color_picture(p)) return p_copy;
    if(is_gray_picture(p)){
        p_copy.channel = 3;
        byte* new_tbl = malloc(byte_number(p_copy)*sizeof(byte));
        for(int i=0; i<p_copy.height*p_copy.width; i++){
            // On initialise un triplet avec les mêmes valeurs que dans la gray picture.
            new_tbl[pixel(i,p_copy.channel,RED)]=p.tbl[i];
            new_tbl[pixel(i,p_copy.channel,GREEN)]=p.tbl[i];
            new_tbl[pixel(i,p_copy.channel,BLUE)]=p.tbl[i];
        }
        p_copy.tbl=new_tbl;
        return p_copy;
    }
    // Si la picture n'est ni coloré ni grise on revoie une picture vide.
    p_copy = create_picture(0,0,0,MAX_BYTE);
    return p_copy;
}

/*  @requires p a picture;
    @assigns nothing;
    @ensures on sucess it returns p converted into gray format. On failure it returns an empty picture; */
picture convert_to_gray_picture(picture p){
    picture p_copy = copy_picture(p);
    // Si la picture est déjà grise on renvoie la copie
    if(is_gray_picture(p)) return p_copy;
    if(is_color_picture(p)){
        p_copy.channel = 1;
        byte* new_tbl = malloc(byte_number(p_copy)*sizeof(byte));
        for(int i=0; i<byte_number(p_copy); i++){
            // Calcule la valeur de chaque pixel à partir des valeurs du triplet d'origine.
            new_tbl[i]=0.299*p.tbl[pixel(i,3,RED)]+0.587*p.tbl[pixel(i,3,GREEN)]+0.114*p.tbl[pixel(i,3,BLUE)];
        }
        p_copy.tbl=new_tbl;
        return p_copy;
    }
    // Si la picture n'est ni coloré ni grise on revoie une picture vide.
    p_copy = create_picture(0,0,0,MAX_BYTE);
    return p_copy;
}

/*  @requires p a picture;
    @assigns nothing;
    @ensures on sucess it returns a picture table which contains the R V B channels of p. On failure it returns NULL; */
picture* split_picture(picture p){
    // Si l'iamge est en niveau de gris on renvoie un tableau ne contenant qu'un seul élément.
    if(is_gray_picture(p)){
        picture* tbl_image = malloc(sizeof(picture));
        *tbl_image = copy_picture(p);
        return tbl_image;
    }
    // Si l'image n'est pas une image en couleur on renvoie NULL.
    if(!is_color_picture(p)) return NULL;
    // Dans la suite, l'image est donc une image en couleur.
    picture* tbl_image = malloc(3*sizeof(picture));
    tbl_image[0] = create_picture(p.width,p.height,1,MAX_BYTE);
    tbl_image[1] = create_picture(p.width,p.height,1,MAX_BYTE);
    tbl_image[2] = create_picture(p.width,p.height,1,MAX_BYTE);
    // On prend tbl_image[0] mais 1 et 2 marchent aussi
    for(int i=0; i<byte_number(tbl_image[0]); i++){
        // Pour chaque triplets chaque éléments sont stocké dans une image.
        tbl_image[0].tbl[i] = p.tbl[pixel(i,3,RED)];
        tbl_image[1].tbl[i] = p.tbl[pixel(i,3,GREEN)];
        tbl_image[2].tbl[i] = p.tbl[pixel(i,3,BLUE)];
    }
    return tbl_image;
}

/*  @requires three gray pictures representing the R V B components of a colored picture;
    @assigns nothing;
    @ensures on sucess it returns the composed colored picture. On failure it returns an empty picture; */
picture merge_picture(picture red, picture green, picture blue){
    // Si au moins une image n'est pas en niveaux de gris.
    int condition1 = !(is_gray_picture(red)&&is_gray_picture(green)&&is_gray_picture(blue));
    // Si au moins une image n'est pas de la même hauteur (resp. largeur) qu'une autre.
    int condition2 = !(((red.height==green.height)&&(green.height==blue.height)));
    int condition3 = !(((red.width==green.width)&&(green.width==blue.width)));

    // Si un de ces cas est vrai on renvoie l'image vide.
    if(condition1||condition2||condition3) return create_picture(0,0,0,MAX_BYTE);
    
    // Sinon on compose l'image.
    picture p = create_picture(red.width,red.height,3,MAX_BYTE);
    
    for(int i=0; i<p.width*p.height; i++){
        p.tbl[i*3+0] = red.tbl[i];
        p.tbl[i*3+1] = green.tbl[i];
        p.tbl[i*3+2] = blue.tbl[i];
    }
    return p;
}

/*  @requires a picture p and a double factor which is the factor to be applied to the pixel values of the image;
    @assigns nothing;
    @ensures returns the brightened image; */
picture brighten_picture(picture p, double factor){
    int new_value;
    picture new_p = copy_picture(p);
    for (int i = 0; i<byte_number(p); i++) {
        // On multiplie la valeur du pixel par le facteur.
        new_value = new_p.tbl[i]*factor;
        // Si le pixel dépasse la valeur maximal il prend la valeur maximal.
        if(new_value>MAX_BYTE) new_p.tbl[i] = MAX_BYTE;
        else new_p.tbl[i] = new_value;
    }
    return new_p;
}

/*  @requires a picture p and an integer representing the number of pixels chosen randomly;
    @assigns nothing;
    @ensures returns the image containing the melted pixels; */
picture melt_picture(picture p, int number) {
    picture new_p = copy_picture(p);
    // Initialisation de la variable contenant le pixel choisi aléatoirement.
    int comp;
    // Initialisation de la variable contenant la somme des coposantes de ce pixel.
    int sum_comp;
    // Initialisation de la variable contenant la somme des composantes du pixel au-dessus de celui choisi.
    int sum_comp_up;
    for (int i = 0; i < number; i++) {
        // On choisit le pixel aléatoirement.
        comp = rand_up_to(p.width * p.height - 1);
        if (comp >= new_p.width) {
            sum_comp = 0;
            sum_comp_up = 0;
            // On calcule la somme des composantes de chaque pixels.
            for (int j = 0; j < new_p.channel; j++){
                sum_comp += new_p.tbl[new_p.channel*comp + j];
                sum_comp_up += new_p.tbl[new_p.channel * (comp - new_p.width) + j];
            }
            // On compare ces deux sommes pour savoir quel pixel est le plus fonçée.
            if (sum_comp_up < sum_comp){
                for (int j = 0; j < new_p.channel; j++) {
                    new_p.tbl[new_p.channel*comp + j] = new_p.tbl[new_p.channel * (comp - new_p.width) + j];
                }
            }
        }
    }
    return new_p;
}

/*  @requires a picture p;
    @assigns nothing;
    @ensures returns the inversed image; */
picture inverse_picture(picture p){
    // Si l'image est video on l'a renvoie.
    if(is_empty_picture(p)) return p;
    picture inv_p = copy_picture(p);
    for (int i = 0; i<byte_number(p); i++) {
        // On inverse la valeur de chaque pixel.
        inv_p.tbl[i] = MAX_BYTE-inv_p.tbl[i];
    }
    return inv_p;
}

/*  @requires a picture p;
    @assigns nothing;
    @ensures returns the normalized picture; */
picture normalize_dynamic_picture(picture p){
    picture norm_p = copy_picture(p);
    // Initialisation des valeurs de min et max.
    byte max = norm_p.tbl[0];
    byte min = norm_p.tbl[0];

    // Recherche des valeurs min et max dans l'image.
    for (int i = 0; i<byte_number(p); i++) {
        if(max<p.tbl[i]) max = p.tbl[i];
        if(min>p.tbl[i]) min = p.tbl[i];
    }

    // Normalisation des pixels de l'image.
    for (int i = 0; i<byte_number(p); i++) {
        norm_p.tbl[i] = (norm_p.tbl[i]-min)*255/(max-min);
    }
    return norm_p;
}

/*  @requires a picture p;
    @assigns nothing;
    @ensures returns the levels adjusted picture; */
picture set_levels_picture(picture p, byte nb_levels){
    picture p_lvl = copy_picture(p);
    // Creation de la lut.
    lut l = create_lut(nb_levels-1);
    // Initialisation des valeurs de la lut.
    for (int i = 0; i < nb_levels; i++) l.value[i] = i+1;
    // Application de la lut.
    p_lvl = apply_lut(l,p_lvl);
    // Normalisation de l'image avec la lut appliquée.
    p_lvl = normalize_dynamic_picture(p_lvl);
    // Libération de la mémoire assigné à la lut.
    clean_lut(&l);
    return p_lvl;
}

/*  @requires two pictures;
    @assigns nothing;
    @ensures returns the image where each pixel contains the normalized differences between the two pictures; */
picture soustr_picture(picture p1, picture p2){
    // On regarde si les deux image sont en couleur.
    int condition1 = is_color_picture(p1)&&is_color_picture(p2);
    // On regarde si les deux image sont en nuances de gris.
    int condition2 = is_gray_picture(p1)&&is_gray_picture(p2);
    // On regarde si les deux image ont des types différent.
    if(!(condition1||condition2)) return create_picture(0,0,0,MAX_BYTE);
    picture p = copy_picture(p1);
    int diff;
    // Initialisation des pixels de la photo contenant la différence.
    for (int i = 0; i<byte_number(p); i++) {
        // On calcule la différence normalisé entre chaque bytes des deux images de base.
        diff = p.tbl[i]-p2.tbl[i];
        if(diff>=0) p.tbl[i]=diff;
        else p.tbl[i]=-diff;
    }
    return p;
}

/*  @requires two pictures;
    @assigns nothing;
    @ensures returns an image where each pixel is the result of the multiplication of the corresponding pixels from the two images; */
picture mult_picture(picture p1, picture p2){
    // On regarde si les deux image sont en couleur.
    int condition1 = is_color_picture(p1)&&is_color_picture(p2);
    // On regarde si les deux image sont en nuances de gris.
    int condition2 = is_gray_picture(p1)&&is_gray_picture(p2);
    // Si les deux image ont des types différent on renvoie une image vide.
    if(!(condition1||condition2)) return create_picture(0,0,0,MAX_BYTE);
    picture p = copy_picture(p1);
    // On crée une liste qui contiendra les multiplications.
    int* tab = malloc(byte_number(p)*sizeof(int));
    // On multiplie les bytes entre eux.
    for (int i = 0; i<byte_number(p); i++) {
        tab[i] = p1.tbl[i]*p2.tbl[i];
    }
    // On initialise le max et le min.
    int max = tab[0];
    int min = tab[0];
    // On cherche le max et le min.
    for (int i = 0; i<byte_number(p); i++) {
        if(max<tab[i]) max = tab[i];
        if(min>tab[i]) min = tab[i];
    }
    // Calcule des valeurs de chaque bytes normalisé entre 0 et 255.
    for (int i = 0; i<byte_number(p); i++) {
        p.tbl[i] = (tab[i]-min)*MAX_BYTE/(max-min);
    }
    // Libération de la mémoire alloué à la liste.
    free(tab);

    return p;
}

/*  @requires three pictures;
    @assigns nothing;
    @ensures returns an image where each pixel is a blend of the first two images based on the corresponding pixel values of the third image; */
picture mix_picture(picture p1, picture p2,picture p3){
    // On vérifie si les trois images n'ont pas la même hauteur.
    int condition1 = !((p1.height==p2.height) && (p2.height==p3.height));
    // On vérifie si les trois images n'ont pas la même largeur.
    int condition2 = !((p1.width==p2.width) && (p2.width==p3.width));
    // Si les images n'ont pas la même hauteur et largeur on renvoie une image vide.
    if(condition1||condition2) return create_picture(0,0,0,MAX_BYTE);
    // Copie des images et conversion de celles-ci en couleur.
    picture p1c = copy_picture(p1);
    p1c = convert_to_color_picture(p1c);
    picture p2c = copy_picture(p2);
    p2c = convert_to_color_picture(p2c);
    picture p3c = copy_picture(p3);
    p3c = convert_to_color_picture(p3c);
    // Création de l'image de sortie.
    picture p = copy_picture(p1c);
    int alpha;
    // Initialisation des valeurs des bytes de l'image de sortie.
    for (int i = 0; i<byte_number(p); i++) {
        alpha = p3c.tbl[i]/255;
        p.tbl[i] = (1-alpha)*p1c.tbl[i]+alpha*p2c.tbl[i];
    }
    // Si les images de bases sont en nuances de gris on renvoie une image en nuances de gris.
    if(p1.channel==1 && p2.channel==1){
        return convert_to_gray_picture(p);
    }
    //Sinon on retourne simplement l'image de sortie.
    else{
        return p;
    }
}

/*  @requires an integer;
    @assigns nothing;
    @ensures returns a random integer between 0 and the integer given; */
int rand_up_to(int max){
  int r;
  do
    r = rand();
  while (r >= RAND_MAX - (RAND_MAX % (max + 1)));
  return r % (max + 1);
}

/*  @requires a picture p;
    @assigns nothing;
    @ensures returns the number of bytes in the picture p; */
int byte_number(picture p){
    return p.height*p.width*p.channel;
}

/*  @requires a picture p and a double factor;
    @assigns nothing;
    @ensures returns the scaled-up picture p with the given factor, using nearest neighbor interpolation; */
picture grow_size_nearest_neighbor_interpolation(picture p, double factor){
    // Calcul des nouvelles dimensions de l'image agrandie.
    int new_width;
    new_width = p.width*factor;
    int new_height;
    new_height = p.height*factor;
    // Création d'une nouvelle image vide avec les nouvelles dimensions et les mêmes paramètres.
    picture result = create_picture(new_width,new_height,p.channel,MAX_BYTE);
    // Indice de ligne correspondant dans l'image originale.
    int ni;
    // Indice de colonne correspondant dans l'image originale
    int nj;
    // Parcours de chaque pixel de l'image agrandie
    for (int i = 0; i<new_height; i++) {
        
        for (int j = 0; j<new_width; j++) {
            // Calcul des indices correspondants dans l'image originale.
            ni = i/factor;
            nj = j/factor;

            // Gestion des cas où les indices calculés dépassent les dimensions de l'image originale.
            if(ni>p.height) ni = p.height-1;
            if(nj>p.width) nj = p.width-1;

            // Copie des valeurs des canaux dans la nouvelle image.
            for(int k = 0; k<result.channel; k++){
                result.tbl[pixel(i * new_width + j, result.channel, k)] = p.tbl[pixel(ni * p.width + nj, result.channel, k)];
            }
        } 
    }
    return result;
}

/*  @requires a picture p and a double factor;
    @assigns nothing;
    @ensures returns the scaled-down picture p with the given factor, using nearest neighbor interpolation; */
picture reduce_size_nearest_neighbor_interpolation(picture p, double factor){
    // Calcul du facteur adéquat pour utiliser la fonction d'agrandissement.
    double new_factor = 1.0/factor;
    // Utilisation de la fonction.
    picture result = grow_size_nearest_neighbor_interpolation(p,new_factor);
    // On retourne l'image réduit qui correspond.
    return result;
}

/*  @requires a picture p and a double factor;
    @assigns nothing;
    @ensures returns the scaled-up picture p with the given factor, using bi-linear interpolation; */
picture grow_size_bi_linear_interpolation(picture p, double factor){
    // Calcul des nouvelles dimensions de l'image agrandie.
    int new_width;
    new_width = p.width*factor;
    int new_height;
    new_height = p.height*factor;
    // Création d'une nouvelle image vide avec les nouvelles dimensions et les mêmes paramètres.
    picture result = create_picture(new_width,new_height,p.channel,MAX_BYTE);
    // Variables pour stocker les pixels voisins à interpoler.
    int p1;
    int p2;
    int p3;
    int p4;
    // Coordonnées fractionnaires des pixels dans l'image originale.
    double x;
    double y;
    // Coordonnées entières des pixels dans l'image originale.
    int int_x;
    int int_y;
    // Coefficients d'interpolation pour le calcul des pixels intermédiaires.
    double a;
    double b;

    // Pour chaque pixel de la nouvelle image.
    for (int i = 0; i<new_height; i++) {
        for (int j = 0; j<new_width; j++) {
            // Calcul des coordonnées fractionnaires (x, y) du pixel dans l'image originale
            x = i/factor;
            y = j/factor;
            // Conversion des coordonnées fractionnaires en entières.
            int_x = x;
            int_y = y;
            // On s'assure que les indices ne dépassent pas les dimensions de l'image originale.
            if (int_x >= p.height) int_x = p.height - 1;
            if (int_y >= p.width) int_y = p.width - 1;
            // Calcule des coefficients d'interpolation.
            a = x - int_x;
            b = y - int_y;

            // Pour chaque composantes.
            for (int c = 0; c < result.channel; c++) {
                // Récupération des pixels voisins.
                p1 = p.tbl[(int_x * p.width + int_y)*p.channel + c];
                p2 = p.tbl[((int_x+1) * p.width + int_y)*p.channel + c];
                p3 = p.tbl[(int_x * p.width + (int_y+1))*p.channel + c];
                p4 = p.tbl[((int_x+1) * p.width + (int_y+1))*p.channel + c];
                // Initialisation de la valeur du bytes
                result.tbl[(i * new_width + j)*p.channel + c] = (1-a)*(1-b)*p1 + a*(1-b)*p2 + (1-a)*b*p3 + a*b*p4;
            }
        }
    }

    return result;
}

/*  @requires a picture p and a double factor;
    @assigns nothing;
    @ensures returns the scaled-down picture p with the given factor, using bi-linear interpolation; */
picture reduce_size_bi_linear_interpolation(picture p, double factor) {
    // Calcul du facteur adéquat pour utiliser la fonction d'agrandissement.
    double new_factor = 1.0/factor;
    // On retourne l'image réduit en utilisant la fonction de grandissement qui correspond.
    return grow_size_bi_linear_interpolation(p,new_factor);
}