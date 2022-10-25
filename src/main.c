#include <stdio.h>
#include <stdlib.h>

struct kayit
{
    int ogrNo;
    int dersKodu;
    int puan;
};

void indexDosyasiOlustur()
{
    FILE *id;
    FILE *vd;

    int boyut=0;
    int i=0;
    int j=0;
    int temp=0;

    if((id=fopen("index.txt","w"))==NULL){
        printf("Hata: Index Dosyasi Olusturulamadi!\n");
        exit(1);
    }

    if((vd=fopen("veri.bin","rb"))==NULL){
        printf("Hata: Veri Dosyasi Okunamadi!\n");
        exit(1);
    }

    while(1){
        struct kayit say;

        size_t s=fread(&say,sizeof(struct kayit),1,vd);

        if(s<1)
            break;

        boyut++;
    }

    fseek(vd,0,SEEK_SET);

    int *sirala;
    sirala=malloc(boyut*2*sizeof(int));

    while(1){
        struct kayit ara;

        size_t s=fread(&ara,sizeof(struct kayit),1,vd);

        if (s<1)
            break;

        sirala[i]=ara.ogrNo;
        sirala[i+1]=(ftell(vd)-sizeof(struct kayit));
        i=i+2;
    }

    fclose(vd);

    for(i=1;i<(boyut*2)-1;i=i+2){
        for(j=0;j<((boyut*2)-1)-i;j=j+2){
            if(sirala[j]>sirala[j+2]){
                temp=sirala[j];
                sirala[j]=sirala[j+2];
                sirala[j+2]=temp;

                temp=sirala[j+1];
                sirala[j+1]=sirala[j+3];
                sirala[j+3]=temp;
            }
        }
    }

    for(i=0;i<(boyut*2)-1;i=i+2){
        fprintf(id,"%d %d\n",sirala[i],sirala[i+1]);
    }

    printf("Index Dosyasi Olusturuldu!\n");

    free(sirala);
    fclose(id);
}

void kayitEkle()
{
    FILE *vd;

    struct kayit yeni;

    printf("Ogrenci No: Ders Kodu: Puan: ");
    scanf("%d %d %d",&yeni.ogrNo,&yeni.dersKodu,&yeni.puan);

    if((vd=fopen("veri.bin","ab"))==NULL){
        printf("Hata: Dosya Acilamadi!\n");
        exit(1);
    }

    fwrite(&yeni,sizeof(struct kayit),1,vd);

    fclose(vd);

    indexDosyasiOlustur();
}

void kayitBul()
{
    int aranan=0;
    printf("Ogrenci Numarasi Giriniz: ");
    scanf("%d",&aranan);

    FILE *id;

    if((id = fopen("index.txt","r")) == NULL){
        printf("Hata: Index Dosyasi Okunamadi!\n");
        exit(1);
    }

    int temp=0;
    int i=0;
    int kontrol=1;
    int boyut=0;
    int bas=0;
    int son=0;
    int orta=0;
    int kayit_sayisi=1;
    int onlem=0;

    while(!feof(id)){
        temp=kontrol;
        fscanf(id,"%d%d",&onlem,&kontrol);
        if(kontrol==temp)
            break;
        boyut++;
    }

    fseek(id,0,SEEK_SET);

    int *bul;
    bul=malloc(boyut*sizeof(int));
    int *os;
    os=malloc(boyut*sizeof(int));
    os[0]=1;

    while(!feof(id)){
        temp=os[i];
        fscanf(id,"%d%d",&(bul[i]),&(os[i]));
        if(os[i]==temp)
            break;
        i++;
    }

    bas=0;
    son=boyut-1;
    orta=(bas+son)/2;

    while(bas<=son){
        if(bul[orta]<aranan)
            bas=orta+1;
        else if(bul[orta]==aranan){
            if(bul[orta-1]==aranan){
                son=orta-1;
                orta=(bas+son)/2;
                continue;
            }
            while(bul[orta+kayit_sayisi]==aranan){
                kayit_sayisi++;
            }
            printf("\n%d Numarali Ogrenciye Ait %d Tane Kayit Bulundu!\n\n",bul[orta],kayit_sayisi);
            break;
        }
        else
            son=orta-1;

        orta=(bas+son)/2;
    }
    if(bas>son)
        printf("\nAradiginiz numara kayitlarda bulunamadi.\n");
    else{
        FILE *vd;

        if((vd=fopen("veri.bin","rb"))==NULL){
            printf("Hata: Veri Dosyasi Okunamadi!\n");
            exit(1);
        }

        printf("OgrNo DersKodu Puan\n");
        printf("----- -------- ----\n");

        for(i=0;i<kayit_sayisi;i++){
            fseek(vd,os[orta+i],SEEK_SET);

            struct kayit liste;

            size_t s=fread(&liste,sizeof(struct kayit),1,vd);

            if(s<1)
                break;

            printf("%5d %8d %4d\n",liste.ogrNo,liste.dersKodu,liste.puan);
        }

        fclose(vd);
    }

    free(bul);
    free(os);
    fclose(id);
}

void kayitSil()
{
    int aranan=0;
    printf("Ogrenci Numarasi Giriniz: ");
    scanf("%d",&aranan);

    FILE *id;

    if((id=fopen("index.txt","r"))==NULL){
        printf("Hata: Index Dosyasi Okunamadi!\n");
        exit(1);
    }

    int temp=0;
    int i=0;
    int kontrol=1;
    int boyut=0;
    int bas=0;
    int son=0;
    int orta=0;
    int kayit_sayisi=1;
    int onlem=0;

    while(!feof(id)){
        temp=kontrol;
        fscanf(id,"%d%d",&onlem,&kontrol);
        if(kontrol==temp)
            break;
        boyut++;
    }

    fseek(id,0,SEEK_SET);

    int *bul;
    bul=malloc(boyut*sizeof(int));
    int *os;
    os=malloc(boyut*sizeof(int));
    os[0]=1;

    while(!feof(id)){
        temp=os[i];
        fscanf(id,"%d%d",&(bul[i]),&(os[i]));
        if(os[i]==temp)
            break;
        i++;
    }

    bas=0;
    son=boyut-1;
    orta=(bas+son)/2;

    while(bas<=son){
        if(bul[orta]<aranan)
            bas=orta+1;
        else if(bul[orta]==aranan){
            if(bul[orta-1]==aranan){
                son=orta-1;
                orta=(bas+son)/2;
                continue;
            }
            while(bul[orta+kayit_sayisi]==aranan){
                kayit_sayisi++;
            }
            printf("\n%d Numarali Ogrenciye Ait %d Tane Kayit Bulundu!\n\n",bul[orta],kayit_sayisi);
            break;
        }
        else
            son=orta-1;

        orta=(bas+son)/2;
    }
    if(bas>son)
        printf("\nAradiginiz numara kayitlarda bulunamadi.\n");
    else{
        FILE *gecici;
        FILE *vd;

        if((vd=fopen("veri.bin","rb"))==NULL){
            printf("Hata: Veri Dosyasi Okunamadi!\n");
            exit(1);
        }

        if((gecici=fopen("gecici.bin","wb"))==NULL){
            printf("Hata: Gecici Dosya Olusturulamadi!\n");
            exit(1);
        }

        printf("OgrNo DersKodu Puan\n");
        printf("----- -------- ----\n");

        for(i=0;i<kayit_sayisi;i++){
            fseek(vd,os[orta+i],SEEK_SET);

            struct kayit liste;

            size_t s=fread(&liste,sizeof(struct kayit),1,vd);

            if(s<1)
                break;

            printf("%5d %8d %4d\n",liste.ogrNo,liste.dersKodu,liste.puan);
        }
        printf("\n");
        int secim;

        while(1){
            printf("1: Ogrenciye Ait Tum Kayitlari Sil\n");
            printf("2: Tek Bir Ders Kaydini Silin\n");

            scanf("%d",&secim);

            if(secim==1||secim==2)
                break;

            printf("Yanlis Secim Yaptiniz\n");
            printf("\n");
        }

        switch(secim){
            case 1:{
                fseek(vd,0,SEEK_SET);
                while(1){
                    struct kayit yeni;

                    size_t s=fread(&yeni,sizeof(struct kayit),1,vd);

                    if (s<1)
                        break;

                    if(yeni.ogrNo==aranan)
                        continue;

                    fwrite(&yeni,sizeof(struct kayit),1,gecici);
                }
                break;
            }
            case 2:{
                int dk=0;
                printf("\nSilmek Istediginiz Puanin Ders Kodunu Giriniz: ");
                scanf("%d",&dk);

                fseek(vd,0,SEEK_SET);
                while(1){
                    struct kayit yeni;

                    size_t s=fread(&yeni,sizeof(struct kayit),1,vd);

                    if (s<1)
                        break;

                    if(yeni.ogrNo==aranan){
                        if(yeni.dersKodu==dk)
                            continue;
                    }

                    fwrite(&yeni,sizeof(struct kayit),1,gecici);
                }
                break;
            }
        };

        fclose(vd);
        fclose(gecici);

        if(remove("veri.bin")!=0){
            printf("Hata: Gecici dosya silinemedi!");
            exit(1);;
        }

        if(rename("gecici.bin","veri.bin")==0)
            printf("Kayit Basarili Bir Sekilde Silindi!\n");
        else{
            printf("Hata: Dosya Ismi Degistirilemedi!\n");
            exit(1);
        }
    }
    free(bul);
    free(os);
    fclose(id);
    indexDosyasiOlustur();
}

void kayitGuncelle()
{
    int aranan=0;
    printf("Ogrenci Numarasi Giriniz: ");
    scanf("%d",&aranan);

    FILE *id;

    if((id=fopen("index.txt","r"))==NULL){
        printf("Hata: Index Dosyasi Okunamadi!\n");
        exit(1);
    }

    int temp=0;
    int i=0;
    int kontrol=1;
    int boyut=0;
    int bas=0;
    int son=0;
    int orta=0;
    int kayit_sayisi=1;
    int onlem=0;

    while(!feof(id)){
        temp=kontrol;
        fscanf(id,"%d%d",&onlem,&kontrol);
        if(kontrol==temp)
            break;
        boyut++;
    }

    fseek(id,0,SEEK_SET);

    int *bul;
    bul=malloc(boyut*sizeof(int));
    int *os;
    os=malloc(boyut*sizeof(int));
    os[0]=1;

    while(!feof(id)){
        temp=os[i];
        fscanf(id,"%d%d",&(bul[i]),&(os[i]));
        if(os[i]==temp)
            break;
        i++;
    }

    bas=0;
    son=boyut-1;
    orta=(bas+son)/2;

    while(bas<=son){
        if(bul[orta]<aranan)
            bas=orta+1;
        else if(bul[orta]==aranan){
            if(bul[orta-1]==aranan){
                son=orta-1;
                orta=(bas+son)/2;
                continue;
            }
            while(bul[orta+kayit_sayisi]==aranan){
                kayit_sayisi++;
            }
            printf("\n%d Numarali Ogrenciye Ait %d Tane Kayit Bulundu!\n\n",bul[orta],kayit_sayisi);
            break;
        }
        else
            son=orta-1;

        orta=(bas+son)/2;
    }
    if(bas>son)
        printf("\nAradiginiz numara kayitlarda bulunamadi.\n");
    else{
        FILE *gecici;
        FILE *vd;

        if((vd=fopen("veri.bin","rb"))==NULL){
            printf("Hata: Veri Dosyasi Okunamadi!\n");
            exit(1);
        }

        if((gecici=fopen("gecici.bin","wb"))==NULL){
            printf("Hata: Gecici Dosya Olusturulamadi!\n");
            exit(1);
        }

        printf("OgrNo DersKodu Puan\n");
        printf("----- -------- ----\n");

        for(i=0;i<kayit_sayisi;i++){
            fseek(vd,os[orta+i],SEEK_SET);

            struct kayit liste;

            size_t s=fread(&liste,sizeof(struct kayit),1,vd);

            if(s<1)
                break;

            printf("%5d %8d %4d\n",liste.ogrNo,liste.dersKodu,liste.puan);
        }
        printf("\n");

        int dk=0;
        int yenipuan=0;

        printf("Guncellemek Istediginiz Puanin Ders Kodunu Girin: ");
        scanf("%d",&dk);
        printf("Puan Girin: ");
        scanf("%d",&yenipuan);

        fseek(vd,0,SEEK_SET);
        while(1){
            struct kayit yeni;

            size_t s=fread(&yeni,sizeof(struct kayit),1,vd);

            if (s<1)
                break;

            if(yeni.ogrNo==aranan){
                if(yeni.dersKodu==dk)
                    yeni.puan=yenipuan;
            }

            fwrite(&yeni,sizeof(struct kayit),1,gecici);
        }

        fclose(vd);
        fclose(gecici);

        if(remove("veri.bin")!=0){
            printf("Hata: Gecici dosya silinemedi!");
            exit(1);;
        }

        if(rename("gecici.bin","veri.bin")==0)
            printf("Kayit Basarili Bir Sekilde Guncellendi!\n");
        else{
            printf("Hata: Dosya Ismi Degistirilemedi!\n");
            exit(1);
        }
    }
    free(bul);
    free(os);
    fclose(id);
    indexDosyasiOlustur();
}

void veriDosyasiniGoster()
{
    FILE *vd;

    if((vd=fopen("veri.bin","rb"))==NULL){
        printf("Hata: Dosya Acilamadi!\n");
        exit(1);
    }

    printf("OgrNo DersKodu Puan\n");
    printf("----- -------- ----\n");

    while(1){
        struct kayit liste;

        size_t s=fread(&liste,sizeof(struct kayit),1,vd);

        if(s<1)
            break;

        printf("%5d %8d %4d\n",liste.ogrNo,liste.dersKodu,liste.puan);
    }

    fclose(vd);
}

void indexDosyasiniGoster()
{
    FILE *id;
    int anahtar=0;
    int offset=1;
    int temp=0;

    if((id=fopen("index.txt","r"))==NULL){
        printf("Hata: Index Dosyasi Okunamadi!\n");
        exit(1);
    }

    printf("Anahtar Offset\n");
    printf("------- ------\n");

    while(!feof(id)){
        temp=offset;
        fscanf(id,"%d%d",&anahtar,&offset);
        if(offset==temp)
            break;
        printf("%7d %6d\n",anahtar,offset);
    }

    fclose(id);
}

void indexDosyasiniSil()
{
    if(remove("index.txt")==0){
        printf("Dosya silindi.\n");
    }else{
        printf("Dosya silinemedi.\n");
        exit(1);
    }
}

int main()
{
    int secim;

    while(1){
        printf("1: Index Dosyasi Olustur\n");
        printf("2: Kayit Ekle\n");
        printf("3: Kayit Bul\n");
        printf("4: Kayit Sil\n");
        printf("5: Kayit Guncelle\n");
        printf("6: Veri Dosyasini Goster\n");
        printf("7: Index Dosyasini Goster\n");
        printf("8: Index Dosyasini Sil\n");
        printf("0: Cikis\n");
        printf("Bir Secim Yapiniz: ");

        scanf("%d",&secim);
        printf("\n");

        if(secim==0)
            exit(1);

        switch(secim){
            case 1:{
                indexDosyasiOlustur();
                break;
            }
            case 2:{
                kayitEkle();
                break;
            }
            case 3:{
                kayitBul();
                break;
            }
            case 4:{
                kayitSil();
                break;
            }
            case 5:{
                kayitGuncelle();
                break;
            }
            case 6:{
                veriDosyasiniGoster();
                break;
            }
            case 7:{
                indexDosyasiniGoster();
                break;
            }
            case 8:{
                indexDosyasiniSil();
                break;
            }
            default:
                printf("Yanlis Secim Yaptiniz\n");
        };
        printf("\n");
        system("pause");
        system("cls");
    }
    return 0;
}
