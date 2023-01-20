/*
#############################################################################
# COMP.CS.110 Ohjelmointi 2: Rakenteet                                      #
# Projekti 4: Peli nimeltä 2048                                             #
# Tiedosto: mainwindow                                                      #
# Kuvaus: Toteuttaa graafisen käyttöliittymän 2048-pelille. Kun ohjelma     #
#         käynnistetään, aivan ensimmäiseksi ruudulle piirtyy aloitusruutu. #
#         Ruudun keskellä yläreunassa on <QLabel>-luokan etikettiolio pelin #
#         aloittamisohjeita varten. Tämän alapuolella vasemmalla on         #
#         <QLabel>-luokan "Siemenluku:"-etiketti, ja <QSpinBox>-luokan      #
#         valintalaatikko sen asettamiseen. Näiden alapuolella on           #
#         vastaavalla tavalla "Tavoiteluku:"-etiketti ja valintalaatikko    #
#         sen asettamiseen. Siemenluvulla satunnaisluku-generaattori        #
#         määrittää alustettaville laatoille koordinaatit                   #
#         pelilaudalla, ja tavoiteluvulla määritellään nimensä mukaisesti   #
#         kohta, jossa peli päättyy voittoon. Näiden                        #
#         etiketti-valintalaatikko -käyttöliittymäkomponenttiparien         #
#         oikealla puolella on widgetit pelissä käytettävän pelilaudan      #
#         sivun pituuden asettamiseksi. Sivun pituus määritetään            #
#         <QSlider>-luokan liukusäädinoliolla, jonka yläpuolella on kaksi   #
#         etikettiä. Toisessa etiketeistä lukee mitä tietoa säätimellä      #
#         asetetaan ("Peliruudun sivun pituus") ja toiseen asetetaan        #
#         säätimen senhetkinen arvo ilmaistuna pelilaudan ruutuina          #
#         (oletuksena "4 ruutua" ja liukusäädin säädetty vastaavaan         #
#         asentoon). Edellä esitettyjen komponenttien alapuolella sijaitsee #
#         <QPushButton>-luokan "Valmis"-painonappi, jota painamalla         #
#         siirrytään aloitusruudusta itse peliruutuun, jonka lisäksi        #
#         aloitusruudun widgetit asetetaan pois näkyvistä. Samalla hetkellä #
#         ruudulle piirtyy itse pelissä käytettävä pelilauta laattoineen.   #
#         Pelilauta on toteutettu <QGraphicsView>- ja <QGraphicsScene>-     #
#         luokkien avulla. Pelilaudalla olevat laatat (neliöt) ovat         #
#         <QGraphicsRectItem>-luokan olioita ja kunkin laatan arvo on       #
#         ilmoitettu pelin laatan päälle asetetuilla <QGraphicsTextItem>-   #
#         luokan tekstigrafiikkaolioina. Pelilaudan alapuolella sijaitsee   #
#         ajastin, joka ilmoittaa senhetkiseen pelikierrokseen käytetyn     #
#         ajan kokonaisina minuutteina ja sekunteina. Itse ajastimen        #
#         logiikka on toteutettu <QTimer>-luokan ajastinolion avulla, jonka #
#         muuttamia tietoja esitetään <QLCDNumber>-luokan numeroruuduissa.  #
#         Kummankin numeroruudun oikealla puolella on etiketti kertomassa   #
#         mitä tietoa kussakin ruudussa esitetään. Ajastin-widgettien       #
#         oikealla puolella pelilaudan oikeassa alakulmassa sijaitsee       #
#         <QLabel>-luokan etiketti, jossa ilmoitetaan pelaajan senhetkisen  #
#         kierroksen aikana keräämät pisteet muodossa "<pisteet> pistettä". #
#         Pelissä saa yhden pisteen jokaisesta pelilaudalle lisätystä       #
#         laatasta. Ajastin-widgettien alapuolella sijaitsee                #
#         "Aloita alusta"-painike, jota painamalla avautuu <QMessageBox>-   #
#         luokan viestilaatikko-olio. Painikkeen nimen mukaisesti sitä      #
#         painettaessa peli voidaan aloittaa alusta viestilaatikon "Yes"-   #
#         vaihtoehdolla, tai vaihtoehtoisesti jatkaa käynnissä olevaa peliä #
#         painamalla viestilaatikon "No"-vaihtoehtoa. Pelin alkaessa alusta #
#         peli alkaa uudelleen samasta tilasta kuin alkuruudun "Valmis"-    #
#         painikkeen painamisen jälkeen, mutta pelilaudan laatat ovat nyt   #
#         uusissa koordinaateissa. Pelin päättyessä voittoon tai häviöön    #
#         ruudulle avautuu jälleen viestilaatikko, jossa on tällä kertaa    #
#         vaihtoehtoina pelata uusi peli tai lopettaa pelaaminen. Jos       #
#         valitaan uusi peli, peli alkaa uudelleen samalla tavalla kuin     #
#         "Valmis"-napin painamisen jälkeen, mutta pelilaudan laatat ovat   #
#         eri koordinaateissa. Jos valitaan pelin lopetus, pääikkuna        #
#         sulkeutuu ja peli on päättynyt. Seuraavaksi tarkempaa tietoa itse #
#         pelin käyttöohjeista ja säännöistä sekä tehdyistä suunnittelu-    #
#         päätöksistä ja peliin toteutetuista lisäominaisuuksista.          #
#                                                                           #
# Käyttöohjeet/säännöt:                                                     #
#         Perinteisessä 2048-pelissä pelilautana toimii 4x4-ruudukko, jossa #
#         on liikuteltavia numerolaattoja. Näiden numerolaattojen arvot     #
#         ovat numeron 2 eri potenssien arvoja, esimerkiksi 2, 4, 8, ...    #
#         jne, mutta laatat voivat olla myös tyhjiä. Lisäksi perinteisen    #
#         pelin tavoitteena on nimensä mukaisesti päästä lukuarvoon 2048    #
#         yhdistämällä pelilaudalla olevia laattoja. Pelilaudan laattoja    #
#         on mahdollista liikuttaa ylös, vasemmalle, alas ja oikealle,      #
#         joita vastaavat näppäimistön näppäimet [W], [A], [S] ja [D]. Jos  #
#         laattojen liikuttamisen seurauksena samanarvoiset laatat          #
#         koskettavat toisiaan, esim. kaksi laattaa, joiden kummankin arvo  #
#         on 4, nämä laatat yhdistyvät yhdeksi uudeksi laataksi, jonka      #
#         arvo on kahden laatan summa. Tällä tavalla laattoja               #
#         yhdistelemällä jatketaan pelin pelaamista kunnes saavutetaan      #
#         tavoitearvo tai vaihtoehtoisesti pelilauta täyttyy laatoista,     #
#         jolloin niiden liikuttelu ei ole enään mahdollista. Tavoitearvon  #
#         saavuttamalla voittaa pelin, ja jos edellä kuvatulla tavalla      #
#         pelilauta täyttyy numerolaatoista, häviää pelin. Kummassakin      #
#         tilanteessa ruudulle ilmestyy viestilaatikko, jossa on            #
#         mahdollista aloittaa uusi peli tai lopettaa pelaaminen kokonaan.  #
#         Myös pelin kuluessa uudelleenaloittaminen/resettaaminen on        #
#         mahdollista pelilaudan alapuolella sijaitsevan "Aloita alusta"-   #
#         painikkeen avulla, jota painamalla ruudulle ilmestyy              #
#         viestilaatikko. Pelaajan on mahdollista seurata pelin aikaista    #
#         statistiikkaa kuluneesta ajasta ja kerätyistä pisteistä           #
#         pelilaudan alareunassa sijaitsevista ajastin- ja kokonaispiste-   #
#         widgeteistä. Edellä mainittujen siemen- ja tavoitelukujen         #
#         arvojen sekä pelilaudan sivun pituuden asettaminen onnistuu       #
#         alkuruudun valintalaatikoiden avulla.                             #
#                                                                           #
# Suunnittelupäätökset:                                                     #
#          * NumberTile-luokkaan tehdyt muutokset:                          #
#              - Lisätty Getter-metodi laatan <value_>-attribuutille.       #
#                  o Hyödynnetään mainwindow.cpp tiedostossa                #
#                    <update_gameboard> -metodissa laatan arvojen           #
#                    ilmoittamiseen käyttöliittymässä.                      #
#                  o Hyvin minimaalisella muutoksella luokan julkiseen      #
#                    rajapintaan saadaan laattojen arvot vaivattomasti      #
#                    käyttöön itse luokan ulkopuolella.                     #
#                                                                           #
#              - Lisätty Setter-metodi laatan <value_>-attribuutille.       #
#                  o Hyödynnetään mainwindow.cpp tiedostossa                #
#                    <reset_gameboard> -metodissa laatan arvojen            #
#                    nollaamiseen pelin uudelleenaloituksen yhteydessä.     #
#                  o Hyvin minimaalisella muutoksella luokan julkiseen      #
#                    rajapintaan saadaan laattojen arvot vaivattomasti      #
#                    vaihdettua itse luokan ulkopuolella.                   #
#                                                                           #
#          * Suunnan valinnan toteutus käyttöliittymässä:                   #
#              - Toteutustavaksi valittu nuolinäppäimiä vastaavien          #
#                kirjaimien käyttö ja ylimääräisen "Valmis"-painikkeen      #
#                luominen.                                                  #
#                  o Valittu käytön ja toteutuksen yksinkertaisuuden        #
#                    vuoksi. Näiden lisäksi suurimmalle osalle pelaajista   #
#                    WASD-näppäinten käyttö on jo varmasti entuudestaan     #
#                    tuttua ja yhtä luontevaa verrattuna tavallisiin        #
#                    nuolinäppäimiin.                                       #
#                                                                           #
#          * GameBoard-luokkaan tehdyt muutokset:                           #
#              - Korvattu <SIZE>-kokonaislukuvakio <size_>-attribuutilla.   #
#                  o Mahdollistaa pelilaudan sivun pituuden muuttamisen     #
#                    GameBoard-luokan ulkopuolelta, tässä tapauksessa       #
#                    <MainWindow>-luokasta. Hyödynnetään käyttöliittymän    #
#                    lisäominaisuudessa, jossa pelaaja voi pelata peliä     #
#                    haluamansa kokoisella pelilaudalla määrittämällä       #
#                    laudan koon itse käyttöliittymän aloitusruudusta.      #
#                                                                           #
#              - Lisätty Getter-metodi pelilaudan sivun pituuden            #
#                ilmoittavalle <size_>-attribuutille.                       #
#                  o Pienellä muutoksella julkiseen rajapintaan             #
#                    mahdollistetaan edelleen pääsy attribuuttiin.          #
#                                                                           #
#              - Lisätty Setter-metodi <size_>-attribuutille.               #
#                  o Pienellä muutoksella julkiseen rajapintaan             #
#                    mahdollistetaan pelilaudan sivun pituuden asettaminen  #
#                    ja muuttaminen GameBoard-luokan ulkopuolelta.          #
#                                                                           #
# Lisäominaisuudet:                                                         #
#          * Taustavärin vaihto voiton/häviön/resetin yhteydessä (10 p)     #
#              - Lisäominaisuuden nimen mukaisesti reagoidaan pääikkunan    #
#                ruudun taustavärillä pelaajan voittoon ja häviöön.         #
#                                                                           #
#              - Tämän lisäksi palautetaan pääikkunan taustan väri takaisin #
#                alkuperäiseen arvoonsa, jos pelaaja on voittanut/hävinnyt  #
#                ja päättää aloittaa uuden pelin.                           #
#                                                                           #
#              - Pääikkunan taustaväri asetetaan <QMainWindow>-luokan       #
#                pääikkunaolioon osoittavan <this> -pointterin ja luokan    #
#                julkisen "setStyleSheet()" -funktion avulla.               #
#                  o Esimerkiksi komennolla                                 #
#                    "this->setStyleSheet("background-color: tan");"        #
#                    funktiolle annetaan parametrina merkkijono, jossa      #
#                    kaksoispisteellä erotettuna valitaan ensin värjättävä  #
#                    alue, esim. tausta tai reunaviiva, ja toisena          #
#                    syötetään väri nimivakion avulla, esim. nyt annettu    #
#                    "tan" vastaa ohjelmassa käytettyä ruskeaa taustan      #
#                    värin alkuarvoa.                                       #
#                  o Voitolle hyödynnetty vihreä väri vastaa vakiota        #
#                    "lawngreen" ja punainen vakiota "red".                 #
#                                                                           #
#              - Pääikkunan väri muutetaan alkuarvoonsa olion rakentajassa  #
#                ja "reset_gameboard()" -metodissa. Vihreä voiton väri      #
#                asetetaan "react_to_win()" -metodissa ja häviötä vastaava  #
#                punainen väri asetetaan "react_to_lose()" -metodissa.      #
#                                                                           #
#          * Eri luvuilla on eri värit (10 p)                               #
#              - Jokaisella pelilaudan laatan mahdollisella arvolla on      #
#                määrätty väri, jolla lukuarvoa vastaava laatta värjätään.  #
#                                                                           #
#              - Peruspelissä 11 eri vaihtoehtoa:                           #
#                [2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048].          #
#                Siis kullakin kokonaislukuarvolla oma vastaava väri,       #
#                paitsi kaikilla lukua 2048 suuremmilla luvuilla identtinen #
#                väriarvo.                                                  #
#                                                                           #
#              - Lukuarvoja vastaavat värit sisältävä tietorakenne          #
#                määritelty "mainwindow.cpp" -tiedostossa.                  #
#                                                                           #
#              - Laattojen värin vaihtamisesta vastaa                       #
#                "change_tile_color(int value)" -metodi, joka saa           #
#                parametrinaan laatan kokonaislukuarvon. Metodia kutsutaan  #
#                "update_gameboard()" -metodista jos laatan lukuarvo ei ole #
#                nolla.                                                     #
#                                                                           #
#              - Värien asettamisessa hyödynnetään <QColor> -luokan värejä  #
#                ja itse laatan maalaamisessa <QBrush>-luokan pensseliä.    #
#                                                                           #
#          * Peliruudukon kokoa voi säätää käyttöliittymän kautta (10 p)    #
#              - Pelin aloitusruudussa pelaajalla on mahdollisuus asettaa   #
#                pelilaudan sivun pituus widgettien avulla.                 #
#                  o Aloitusruudun oikella puolella on <QSlider>-luokan     #
#                    liukusäädinolio, jota liu'uttamalla pelaaja voi siis   #
#                    asettaa muodostettavalle pelilaudalle haluamansa       #
#                    sivun pituuden. Liukusäätimen yläpuolella kaksi        #
#                    <QLabel>-luokan etikettiä, joista toisessa kerrotaan   #
#                    mitä liukusäätimellä säädetään, ja toisessa näytetään  #
#                    liukusäätimen sen hetkinen arvo pelilaudan ruutuina.   #
#                                                                           #
#              - Liukusäätimen eli ts. pelilaudan sivun pituuden arvot on   #
#                rajoitettu kokonaislukujoukkoon {3, 4, 5, 6, 7, 8},        #
#                jolloin peli toimii vaihtelevalla määrällä ruutuja, vaikka #
#                <size_>-attribuutin arvoa muutetaan hieman.                #
#                                                                           #
#              - Edellä mainitut widgetit alustetaan                        #
#                "init_board_size_widgets()"-metodissa. Liukusäädinolio     #
#                lähettää sen arvon muutoksen yhteydessä "valueChanged()"-  #
#                signaalin, jonka vastaanottaa                              #
#                "handle_size_slider_movement()"-slotti. Kyseinen metodi    #
#                päivittää liukusäätimen osoittaman ruutujen lukuarvon sen  #
#                kertovaan etikettiin.                                      #
#                                                                           #
#              - Kun pelaaja painaa alkuruudun 'Valmis'-painiketta, nappi   #
#                lähettää tuttuun tapaan signaalin                          #
#                "handle_ready_button_click()"-slottiin. Slotin rakenteessa #
#                liukusäätimen sen hetkinen arvo haetaan ja tallennetaan    #
#                <board_size_>-attribuuttiin, joka annetaan parametrina     #
#                pelilautaolion "set_size(int new_size)"-metodille.         #
#                Näin ollen kun pelilautaa aletaan muodostamaan ja          #
#                täyttämään piirtyy ruudulle juuri oikean/halutun kokoinen  #
#                pelilauta.                                                 #
#                                                                           #
#          * Pelissä on jonkinlainen pistelasku, joka perustuu              #
#            pelaajan taitavuuteen (10 p)                                   #
#              - Lisäominaisuus lisää peliin yksinkertaisen pistelasku-     #
#                järjestelmän. Tässä järjestelmässä pelaaja saa yhden       #
#                pisteen joka kerta kun pelilaudalle lisätään uusi laatta.  #
#                                                                           #
#              - Tämä pistelaskutapa vaatii siis pelaajalta taitoa yhdistää #
#                laudalla mahdollisimman monta laattaa, jotta pelilaudalle  #
#                mahtuisi luonnollisesti mahdollisimman monta uutta         #
#                laattaa.                                                   #
#                                                                           #
#              - Pelaajan kierroksen aikana keräämät pisteet tallennetaan   #
#                <current_player_score_>-attribuuttiin. Pisteet esitetään   #
#                <QLabel>-luokan etiketissä, joka alustetaan                #
#                "init_player_score_widgets()"-metodissa.                   #
#                                                                           #
#              - Pisteitä kasvatetaan julkisen rajapinnan                   #
#                "keyPressEvent(QKeyEvent* event)"-slotissa jokaisella      #
#                kerralla kun pelaaja on painanut yhtä laattojen            #
#                liikuttamiseen tarkoitetuista WASD-näppäimistä, jolloin    #
#                myös pelilaudalle lisätään uusi laatta. Tällöin lähetetään #
#                "increase_score()"-signaali, jonka vastaanottaa            #
#                "handle_score_increase()"-slotti.                          #
#                                                                           #
#              - Juuri mainitussa slotissa muotoillaan <QString>-muuttuja,  #
#                joka asetetaan aiemmin "init_player_score_widgets()"-      #
#                metodissa alustettuun <QLabel>-luokan etikettiin           #
#                loppuliitteen kera.                                        #
#                                                                           #
#          * Pelissä on ajastin, joka kertoo kuluneen ajan (5 p)            #
#              - Lisäominaisuudella lisätään peliin ajastin, josta          #
#                pelaaja voi nähdä yhden pelikierroksen aikana kuluneen     #
#                ajan minuutteina ja sekunteina.                            #
#                                                                           #
#              - Ominaisuuden toteuttamiseen tarvitaan <QTimer>-luokan      #
#                ajastinoliota sekä <QLCDNumber>-luokan numeroruutuja ja    #
#                <QLabel>-luokan etikettejä. Hyödynnetään lisäksi           #
#                <minutes_>- ja <seconds_>-attribuutteja minuuttien ja      #
#                sekuntien kokonaislukuarvojen tallentamiseen.              #
#                  o Kummallekin tyypille, minuuteilla ja sekunneille,      #
#                    luodaan omat numeroruudut ja etiketit. Numeroruuduissa #
#                    esitetään attribuuttien arvot ja etiketeissä kerrotaan #
#                    mitä tietoa numeroruudut esittävät.                    #
#                                                                           #
#                  o Edellä mainitut widgetit alustetaan ja määritetään     #
#                    "init_timer_widgets()"-metodissa.                      #
#                                                                           #
#              - Lisäksi ajastinoliolla on neljä yksityistä slottia,        #
#                "start_timer()", "stop_timer()", "reset_timer()" ja        #
#                "update_lcd()", jotka samassa järjestyksessä vastaavat     #
#                ajastimen käynnistämisestä, pysäytyksestä, nollauksesta    #
#                ja arvojen esittämisestä oikealla tavalla numeroruuduissa. #
#                                                                           #
#              - Kun ajastin käynnistetään, sen "start()"-metodille         #
#                annetaan parametrina <TIMEOUT_DURATION>-kokonaislukuvakio, #
#                joka määrittää ajastimen päivitystiheyden, eli ts. kuinka  #
#                nopeasti aika kuluu.                                       #
#                                                                           #
#              - Edellä kuvatut tiedot esitetään peliruudulla itse          #
#                pelilaudan alapuolella.                                    #
#                                                                           #
# Huomioita:                                                                #
#          * Tämä on osa harjoitusprojektia.                                #
#          * Ohjelma on suoritettu valgrindilla, ja muistinhallintaan       #
#            liittyviä virheitä ei testissä löytynyt (lukuunottamatta       #
#            external erroreita).                                           #
#                                                                           #
# Ohjelman kirjoittaja:                                                     #
#          * Nimi: Saku Hakamäki                                            #
#          * Opiskelijanumero: *******                                      #
#          * Käyttäjätunnus: ******                                         #
#          * E-Mail: *********************                                  #
#                                                                           #
#############################################################################
*/

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

// Sisällytetään GameBoard- ja NumberTile-luokat.
#include "gameboard.hh"
#include "numbertile.hh"

// Sisällytetään muut käyttöliittymän
// rakentamisessa tarvittavat widget-luokat.
#include <QMainWindow>
#include <QDebug>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <cmath>
#include <QKeyEvent>
#include <QMessageBox>
#include <map>
#include <QBrush>
#include <QColor>
#include <QSlider>
#include <QTimer>
#include <QLCDNumber>

// QString-vakioita, joita käytetään käyttöliittymäkomponenteisssa.
const QString START_SCREEN_HEADER_TEXT = "Tervetuloa pelaamaan 2048-peliä!\n"
                                         "Syötä siemen- ja tavoiteluvut "
                                         "sekä pelilaudan sivun pituus.\n"
                                         "Paina 'Valmis' jatkaaksesi.",
              SEED_LABEL_TEXT = "Siemenluku:",
              GOAL_VALUE_TEXT = "Tavoiteluku:",
              READY_BUTTON_TEXT = "Valmis",
              RESET_BUTTON_TEXT = "Aloita alusta",
              SIZE_LABEL_TEXT = "Peliruudukon sivun pituus:",
              DEFAULT_SIZE_VALUE_TEXT = "4 ruutua",
              TAN_BACKGROUND = "background-color: tan",
              WHITE_BACKGROUND = "background-color: white",
              GREEN_BACKGROUND = "background-color: lawngreen",
              RED_BACKGROUND = "background-color: red",
              RESTART_TEXT = "Aloita alusta?",
              ARE_YOU_SURE_TEXT = "Oletko varma, että haluat aloittaa alusta?",
              WIN_TITLE_TEXT = "Voitit saavuttamalla tavoiteluvun!",
              PLAY_AGAIN_TEXT = "Haluatko pelata uudelleen?",
              LOSE_TITLE_TEXT = "Uuden laatan lisääminen epäonnistui, "
                                "hävisit pelin.",
              TRY_AGAIN_TEXT = "Haluatko yrittää uudelleen?",
              MINUTES_TEXT = " minuuttia",
              SECONDS_TEXT = " sekuntia",
              POINTS_SUFFIX = " pistettä",
              RECT_SUFFIX = " ruutua";

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // Rakentaja ja purkaja.
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /**
     * Julkinen slotti, joka vastaanottaa
     * signaalin pelaajan näppäinkomennoista.
     *
     * @param[event] Osoitin näppäinkomentosignaalin lähettäjään
     *               eli näppäimistön näppäimeen.
     */
    void keyPressEvent(QKeyEvent* event);

signals:

    /**
     * Signaali, joka lopettaa/sulkee pelin.
     */
    void quit_game();

    /**
     * Signaali, joka aloittaa pelin alusta.
     */
    void reset_game();

    /**
     * Signaali, joka kasvattaa pelaajan pistemäärää.
     */
    void increase_score();

private slots:

    /**
     * Yksityinen slotti, joka vastaanottaa signaalin pelaajan
     * pistemäärän kasvattamisesta ja muotoilee
     * <player_score_label_>-etiketin uudelleen.
     */
    void handle_score_increase();

    /**
     * Yksityinen slotti, joka vastaanottaa signaalin aloitusruudun
     * liukusäätimeltä sen arvoa muutettaessa.
     */
    void handle_size_slider_movement();

    /**
     * Yksityinen slotti, joka vastaanottaa signaalin 'Valmis'-painikkeelta.
     */
    void handle_ready_button_click();

    /**
     * Yksityinen slotti, joka vastaanottaa pelinaikaisen reset-signaalin.
     */
    void handle_reset_midgame_button_click();

    /**
     * Yksityinen slotti, joka vastaanottaa signaalin
     * pelin resetoinnista pelin ollessa päättynyt.
     */
    void reset_gameboard();

    /**
     * Yksityinen slotti, joka käynnistää ajastimen.
     */
    void start_timer();

    /**
     * Yksityinen slotti, joka pysäyttää ajastimen.
     */
    void stop_timer();

    /**
     * Yksityinen slotti, joka nollaa ajastimen.
     */
    void reset_timer();

    /**
     * Yksityinen slotti, joka päivittää <minutes_>- ja <seconds_>-
     * attribuuttien arvot <QLCDNumber>-luokan widgetolioihin.
     */
    void update_lcd();

private:
    Ui::MainWindow *ui;

    // Kokonaislukuvakio widgettien väliselle etäisyydelle.
    const int MARGIN = 100;

    // Vakioarvot widgettien leveydelle ja korkeudelle.
    const int DEFAULT_WIDGET_WIDTH = 50,
              DEFAULT_WIDGET_HEIGHT = 30;

    // Vakioarvot etikettien leveydelle ja korkeudelle.
    const int DEFAULT_LABEL_WIDTH = 250,
              DEFAULT_LABEL_HEIGHT = 100;

    // Osoittimet aloitusruudussa käytettäviin widgetteihin.
    QLabel* header_label_;
    QLabel* seed_label_;
    QSpinBox* seed_spinbox_;
    QLabel* goal_value_label_;
    QSpinBox* goal_value_spinbox_;
    QPushButton* ready_button_;

    /**
     * Metodi, joka alustaa pelin aloitusruudun
     * ja siinä sijaitsevat widgetit.
     */
    void init_start_screen();

    // Attribuutit, jonne tallennetaan käyttäjän
    // syöttämät arvot siemen- ja tavoiteluvuille.
    int seed_;
    int goal_value_;

    // Osoitin pelissä käytettävään pelilautaolioon.
    GameBoard board_;

    // Osoittimet pelilaudan "maisemaan" ja "näkymään".
    QGraphicsScene* scene_;
    QGraphicsView* graphics_view_;

    /**
     * Metodi, joka rakentaa ja päivittää pelilaudan ruudulle.
     */
    void update_gameboard();

    // Lippuattribuutti, joka kertoo onko pelilauta jo luotu, jotta
    // vältytään sen uudelleenluonnilta ja osoittimien päiden hukkaamiselta.
    bool gameboard_created_ = false;

    // Vakioarvot pelilaudan laattojen sivujen pituudelle ja sijainnille.
    const int TILE_BASE_SIDE_LENGTH = 101,
              TILE_SIDE_LENGTH = 100,
              RECT_COORD = 0;

    // Osoittimet pelilaudan laattoihin ja niiden widgetteihin.
    NumberTile* tile_;
    QGraphicsRectItem* rect_;
    QGraphicsTextItem* tile_text_;

    // Attribuutti, jonne tallennetaan pelaajan
    // haluama laattojen liikutussuunta.
    Coords dir_;

    // Lippuattribuutti, jonne tallennetaan tieto siitä,
    // että onko pelaaja voittanut pelin.
    bool has_won_ = false;

    /**
     * Metodi, jota kutsutaan pelaajan voittaessa pelin.
     */
    void react_to_win();

    /**
     * Metodi, jota kutsutaan kun pelaaja on hävinnyt pelin.
     */
    void react_to_lose();

    /**
     * Metodi, joka tulostaa ruudulle viestilaatikon riippuen
     * pelin tuloksesta.
     *
     * @param[has_lost] bool, onko pelaaja hävinnyt vai voittanut pelin.
     *                        Hyödynnetään sopivan viestilaatikon valinnassa.
     */
    void show_end_screen_message_box(bool has_lost = false);

    // Osoitin pelinaikaiseen reset-painikkeeseen.
    QPushButton* reset_midgame_button_;

    /**
     * Metodi, joka vaihtaa <rect_> -attribuutin osoittaman laatan taustavärin
     * parametrina saamansa laatan lukuarvon mukaiseksi. Asetettavan värin
     * tiedot haetaan TILES_RGB -tietorakenteesta <value>-parametrilla.
     *
     * @param[value] int, maalattavan/värjättävän pelilaudan laatan lukuarvo.
     *                    Hyödynnetään laatan väriarvojen noutamiseksi
     *                    tietorakenteesta.
     */
    void change_tile_color(int value);

    // Attribuutti, jonne tallennetaan pelilautaolion sivun pituus.
    int board_size_;

    // Pelin perusversion pelilaudan sivun pituus.
    const int DEFAULT_BOARD_SIZE = 4;

    /**
     * Metodi, joka alustaa ja määrittää aloitusruudun widgetit, joita
     * käytetään pelilaudan sivun pituuden asettamiseen.
     */
    void init_board_size_widgets();

    // Osoitin aloitusruudun liukusäätimeen, sekä sen
    // info- ja lukuarvoetiketit.
    QSlider* size_slider_;
    QLabel* size_label_;
    QLabel* size_value_label_;

    // Attribuutti, jonne tallennetaan pelaajan
    // yhden pelikierroksen aikana keräämät pisteet.
    int current_player_score_;

    // Osoittaja etikettiin, jossa pelaajan pisteet
    // esitetään pelin kuluessa.
    QLabel* player_score_label_;

    /**
     * Metodi, joka alustaa ja määrittää pelaajan pisteiden
     * esittämiseen vaadittavat widgetit.
     */
    void init_player_score_widgets();

    // Osoitin käyttöliittymän ajastinolioon.
    QTimer* timer_;

    // Kokonaislukuvakio, joka määrittää
    // ajastimen päivitystiheyden millisekunneissa,
    // jolloin 1000 millisekuntia = 1 sekunti.
    const int TIMEOUT_DURATION = 1000;

    // Attribuutit, jonne tallennetaan minuuttien ja sekuntien arvot.
    int minutes_ = 0;
    int seconds_ = 0;

    // Osoittimet kellon näyttämiseen käytettäviin
    // numeroruutuihin ja etiketteihin.
    QLCDNumber* lcd_min_;
    QLabel* min_label_;
    QLCDNumber* lcd_sec_;
    QLabel* sec_label_;

    /**
     * Metodi, joka alustaa ja määrittää peliruudun kellon
     * esittämiseen vaadittavat widgetit.
     */
    void init_timer_widgets();
};
#endif // MAINWINDOW_HH
