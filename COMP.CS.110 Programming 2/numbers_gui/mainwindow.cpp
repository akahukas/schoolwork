/*
#############################################################################
# COMP.CS.110 Ohjelmointi 2: Rakenteet                                      #
# Projekti 4: Peli nimeltä 2048                                             #
# Tiedosto: mainwindow                                                      #
#                                                                           #
# Ohjelman kirjoittaja:                                                     #
#          * Nimi: Saku Hakamäki                                            #
#          * Opiskelijanumero: *******                                      #
#          * Käyttäjätunnus: ******                                         #
#          * E-Mail: *********************                                  #
#                                                                           #
#############################################################################
*/

#include "mainwindow.hh"
#include "ui_mainwindow.h"

// Kokonaislukuparivakiot eri laattojen liikkumissuunnille.
const Coords DEFAULT_DIR = {0, 0},
             LEFT = {0, -1},
             UP = {-1, 0},
             RIGHT = {0, 1},
             DOWN = {1, 0};

// Lyhennetään RGB-väriarvojen tallentamiseen käytettävä tietorakenne.
using RGB_VALUES = std::map<int, std::vector<int>>;

// Tallennetaan laattojen maalaamiseen käytettävät väriarvot
// tietorakenteeseen. Tietorakenteena käytetään <map>-rakennetta,
// jonka avaimena ovat pelin perusversion laattojen mahdolliset
// kokonaislukuarvot. Hyötykuormana on vektori, jonka alkioina
// toimivat RGB-arvot seuraavanlaisesti; {RED, GREEN, BLUE}.
// Tietorakenteen viimeistä arvoa käytetään kaikille laatoille,
// joiden arvo on suurempi kuin 2048. Tietorakenteen rivien
// loppuun kommentoitu kullakin rivillä olevaa RGB-arvoa
// vastaava SVG-värivakion nimi.
const RGB_VALUES TILES_RGB = {{2, {240, 248, 255}},   // aliceblue
                              {4, {224, 255, 255}},   // lightcyan
                              {8, {173, 216, 230}},   // lightblue
                              {16, {135, 206, 235}},  // skyblue
                              {32, {0, 191, 255}},    // deepskyblue
                              {64, {30, 144, 255}},   // dodgerblue
                              {128, {175, 238, 238}}, // paleturquoise
                              {256, {0, 255, 255}},   // cyan
                              {512, {72, 209, 204}},  // mediumturquoise
                              {1024, {0, 206, 209}},  // darkturquoise
                              {2048, {0, 139, 139}},  // darkcyan
                              {4096, {0, 128, 128}}}; // teal

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Luodaan uusi Gameboard-luokan pelilautaolio.
    GameBoard board;
    board_ = board;

    // Haetaan pelilaudan koon oletusarvo.
    board_size_ = board_.get_size();

    // Asetetaan ruudun taustaväri.
    this->setStyleSheet(TAN_BACKGROUND);

    // Määritetään pääikkunan koko riippuvaiseksi pelilaudan koosta.
    this->setGeometry(MARGIN, MARGIN, 2*board_size_*MARGIN,
                      1.5*board_size_*MARGIN);

    // Alustetaan pelin aloitusruutu.
    init_start_screen();

    // Alustetaan pelaajan pisteet.
    current_player_score_ = 0;

    // Yhdistetään pelin lopetussignaali oikeaan slottiin.
    connect(this, SIGNAL(quit_game()), this, SLOT(close()));

    // Yhdistetään pelin reset-signaali oikeaan slottiin.
    connect(this, SIGNAL(reset_game()), this, SLOT(reset_gameboard()));

    // Yhdistetään pelaajan pistemäärää kasvattava signaali slottiin,
    // joka muotoilee ja asettaa uudet pisteet etikettiin.
    connect(this, SIGNAL(increase_score()),
            this, SLOT(handle_score_increase()));

    // Luodaan uusi ajastinolio ja yhdistetään
    // se widgettejä päivittävään slottiin.
    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, &MainWindow::update_lcd);

    // Yhdistetään myös 'Valmis'-painikkeen
    // painallus ajastimen käynnistykseen.
    connect(ready_button_, &QPushButton::clicked,
            this, &MainWindow::start_timer);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // Tarkastellaan komentoja vain jos pelilauta on luotu, jolloin
    // vältytään ohjelman kaatumiselta näppäimiä paineltaessa
    // aloitusruudussa.
    if(gameboard_created_)
    {
        // Testituloste, ilmoitus siitä, että toimiiko signaali oikein.
        // qDebug() << "Painallus";

        // Lippumuuttuja, jolla merkitään onko painettu näppäin sallittu.
        bool suitable_key = false;

        // Asetetaan <dir_>-attribuutin arvo halutun suunnan
        // mukaiseksi ja liikutetaan pelilaudan laattoja
        // kutsumalla pelilautaolion move-metodia ja vaihdetaan lippumuuttujan
        // arvo totuusarvoon <true>.

        // Jos pelaaja haluaa liikuttaa laattoja pelilaudalla ylös.
        if(event->key() == Qt::Key_W)
        {
            dir_ = UP;
            has_won_ = board_.move(dir_, goal_value_);
            suitable_key = true;
        }
        // Jos pelaaja haluaa liikuttaa laattoja pelilaudalla vasemmalle.
        if(event->key() == Qt::Key_A)
        {
            dir_ = LEFT;
            has_won_ = board_.move(dir_, goal_value_);
            suitable_key = true;
        }
        // Jos pelaaja haluaa liikuttaa laattoja pelilaudalla alas.
        if(event->key() == Qt::Key_S)
        {
            dir_ = DOWN;
            has_won_ = board_.move(dir_, goal_value_);
            suitable_key = true;
        }
        // Jos pelaaja haluaa liikuttaa laattoja pelilaudalla oikealle.
        if(event->key() == Qt::Key_D)
        {
            dir_ = RIGHT;
            has_won_ = board_.move(dir_, goal_value_);
            suitable_key = true;
        }
        if(suitable_key)
        {
            // Asetetaan attribuutin arvo takaisin oletusarvoonsa.
            dir_ = DEFAULT_DIR;

            // Tyhjennetään näkymä siinä olevista laatoista.
            scene_->clear();

            // Päivitetään pelilaudan laatat ruudulle uusille paikoilleen.
            update_gameboard();

            // Lisätään laudalle uusi laatta satunnaiseen kohtaan ja
            // kasvatetaan pelaajan pistemäärää yhdellä. Lähetetään signaali
            // tuloksen muutoksen käsittelevälle slotille.
            board_.new_value(false);
            current_player_score_++;
            emit increase_score();

            // Jokaisen siirron jälkeen tarkastellaan, onko pelaaja
            // voittanut tai hävinnyt pelin. Tarkastelun perusteella
            // kutsutaan sopivaa metodia.

            // Jos aiemman siirron perusteella pelilaudan move-metodi on
            // palauttanut true-totuusarvon, kutsutaan voittoon
            // reagoivaa metodia.
            if(has_won_)
            {
                react_to_win();
            }
            // Jos pelaaja ei ole voittanut, mutta pelilauta on kuitenkin
            // täyttynyt, niin seuraavat siirrot eivät ole enään mahdollisia.
            // Tämän vuoksi kutsutaan häviöön reagoivaa metodia.
            else if(board_.is_full())
            {
                react_to_lose();
            }
        }
    }
}

void MainWindow::handle_score_increase()
{
    // Testituloste, toimiiko slotti.
    //qDebug() << "Lisäys";

    // Muotoillaan pelaajan pisteistä ja
    // loppuliitteestä esitys pelaajan alkupisteistä.
    QString score = QString::number(current_player_score_) + POINTS_SUFFIX;

    // Asetetaan uusi pistemäärä etikettiin.
    player_score_label_->setText(score);
}

void MainWindow::handle_size_slider_movement()
{
    // Liukusäätimen liikkuessa haetaan arvo muuttujaan.
    int slider_value = size_slider_->value();

    // Muutetaan tallennettu arvo <QString>-muotoon ja lisätään takaliite.
    QString slider_value_str = QString::number(slider_value) + RECT_SUFFIX;

    // Sijoitetaan tekstimuuttuja etiketin arvoksi.
    size_value_label_->setText(slider_value_str);
}

void MainWindow::handle_ready_button_click()
{
    // Haetaan attribuuttiin arvo liukusäätimen asennosta.
    board_size_ = size_slider_->value();

    // Asetetaan pelilaudaolion <size_>-attribuutti.
    board_.set_size(board_size_);

    // Muutetaan pääikkunan koko uuden pelilaudan koon mukaiseksi.
    this->setGeometry(MARGIN, MARGIN, 1.5*board_size_*MARGIN,
                      1.5*board_size_*MARGIN);

    // Haetaan siemen- ja tavoiteluvut niille
    // tarkoitettuihin attribuutteihin.
    seed_ = seed_spinbox_->value();
    goal_value_ = goal_value_spinbox_->value();

    // Alustetaan pelilautaolio käyttäjän syöttämällä arvolla.
    board_.init_empty();
    board_.fill(seed_);

    // Rakennetaan pelilauta ruudulle.
    update_gameboard();

    // Luodaan pelaajan pistemäärän esittämiseen tarvittavat widgetit.
    init_player_score_widgets();

    // Luodaan ajastimen esittämiseen vaaditut widgetit.
    init_timer_widgets();

    // Testitulosteet tallennetuille arvoille.
    //qDebug() << "Seed:" << seed_;
    //qDebug() << "Goal value:" << goal_value_;

    // Piilotetaan kaikki aloitusruudun widgetit.
    header_label_->setVisible(false);
    seed_label_->setVisible(false);
    seed_spinbox_->setVisible(false);
    goal_value_label_->setVisible(false);
    goal_value_spinbox_->setVisible(false);
    ready_button_->setVisible(false);
    size_slider_->setVisible(false);
    size_label_->setVisible(false);
    size_value_label_->setVisible(false);

    // Asetetaan itse pelilauta ja reset-nappi näkyviin.
    graphics_view_->setVisible(true);
    reset_midgame_button_->setVisible(true);
}

void MainWindow::handle_reset_midgame_button_click()
{
    // Pysäytetään ajastin viestilaatikon ajaksi.
    stop_timer();

    // Muuttuja, jonne tallennetaan pelaajan
    // valinta viestilaatikon vaihtoehdoista.
    int message_box_result = 0;

    // Luodaan viestilaatikko, jossa kysytään
    // pelaajan halukkuutta aloittaa peli alusta.
    message_box_result = QMessageBox::question(nullptr,
                                               RESTART_TEXT,
                                               ARE_YOU_SURE_TEXT,
                                               QMessageBox::Yes,
                                               QMessageBox::No);

    // Jos pelaaja haluaa aloittaa pelin alusta, lähetetään reset-signaali.
    // Muussa tapauksessa peli jatkuu siitä mihin se jäikin.
    if(message_box_result == QMessageBox::Yes)
    {
        emit reset_game();
    }

    // Käynnistetään ajastin uudelleen.
    start_timer();
}

void MainWindow::reset_gameboard()
{
    // Peli alkaa alusta, joten pelaaja
    // ei ole voinut vielä voittaa peliä.
    has_won_ = false;

    // Käydään pelilaudan jokainen x-koordinaatti yksitellen läpi.
    for(int i = 0; i < board_size_; i++)
    {
        // Käydään kussakin x-koordinaatissa eli sarakkeessa
        // sijaitsevat y-koordinaatit läpi yksitellen.
        for(int j = 0; j < board_size_; j++)
        {
            // Luodaan x- ja y-koordinaateista pari, jonka avulla
            // voidaan hakea koordinaateissa sijaitseva pelilaudan laatta.
            std::pair<int, int> coords = std::make_pair(j, i);

            // Haetaan GameBoard-luokan pelilautaoliosta
            // koordinaateissa sijaitseva NumberTile-luokan laattaolio.
            tile_ = board_.get_item(coords);

            // Määritetään laatan uusi arvo.
            int new_tile_value = 0;

            // Hyödynnetään NumberTile-luokan metodia
            // laatan arvon vaihtamiseen.
            tile_->set_value(new_tile_value);

            // Testituloste alustetulle laatan arvolle.
            // qDebug() << tile_->get_value();
        }
    }
    // Koska juuri alustettiin pelilauta, lisätään sinne uusia laattoja
    // samaan tapaan kuin GameBoard-luokan fill-metodi tekee, eli yhtä monta
    // uutta laattaa kuin pelilaudan rivillä on laattoja yhteensä.
    for( int i = 0 ; i < board_size_ ; ++i )
    {
        board_.new_value();
    }
    // Tyhjennetään vanhat laatat pelilaudalta
    // ja päivitetään uudet arvot pelilaudalle.
    scene_->clear();
    update_gameboard();

    // Asetetaan pelilauta ja reset-nappi uudelleen näkyviin ja
    // määritetään ruudun väri takaisin alkuperäiseen arvoonsa.
    graphics_view_->setVisible(true);
    reset_midgame_button_->setVisible(true);
    this->setStyleSheet(TAN_BACKGROUND);

    // Pelin uudelleenaloituksen yhteydessä alustetaan
    // myös pelaajan pisteet. Lähetetään signaali
    // pisteet sisältävän etiketin sisällön päivittämiseksi
    // ja asetetaan etiketti jälleen näkyviin.
    current_player_score_ = 0;
    emit increase_score();
    player_score_label_->setVisible(true);

    // Nollataan ajastin ja asetetaan se jälleen näkyviin,
    // sillä peli alkaa uudelleen.
    reset_timer();
    lcd_min_->setVisible(true);
    min_label_->setVisible(true);
    lcd_sec_->setVisible(true);
    sec_label_->setVisible(true);
}

void MainWindow::start_timer()
{
    // Käynnistetään ajastin vakioarvoisella
    // päivitystiheydellä, eli sekunnin välein.
    timer_->start(TIMEOUT_DURATION);
}

void MainWindow::stop_timer()
{
    // Pysäytetään ajastin.
    timer_->stop();
}

void MainWindow::reset_timer()
{
    // Nollataan kummatkin attribuutit
    // ja asetetaan tieto numeroruuduille.

    minutes_ = 0;
    lcd_min_->display(minutes_);

    seconds_ = 0;
    lcd_sec_->display(seconds_);
}

void MainWindow::update_lcd()
{
    // Attribuutin arvo kasvaa sekunnin välein.
    seconds_++;

    // Jos minuutti on tullut täyteen, kasvatetaan <minutes_>-
    // attribuutin arvoayhdellä ja nollataan sekunnit.
    if(seconds_ == 60)
    {
        minutes_++;
        seconds_ = 0;
    }

    // Asetetaan luvut numeroruutuihin.
    lcd_min_->display(minutes_);
    lcd_sec_->display(seconds_);
}

void MainWindow::init_start_screen()
{
    // Luodaan widgetit pelilaudan koon asettamista varten.
    init_board_size_widgets();

    // Luodaan otsikkoetiketti ja määritetään
    // sen sijainti ja koko ruudulla.
    header_label_ = new QLabel(START_SCREEN_HEADER_TEXT, this);
    header_label_->setGeometry(1.5*MARGIN, 1*MARGIN,
                               2*DEFAULT_LABEL_WIDTH,
                               DEFAULT_LABEL_HEIGHT);

    // Asetetaan etiketti rivittämään teksti. Tässä tapauksessa
    // teksti rivitetään jokaisen rivinvaihtomerkin "\n" kohdalta.
    header_label_->setWordWrap(true);

    // Kohdistetaan etiketin teksti keskelle horisontaalisesti.
    header_label_->setAlignment(Qt::AlignHCenter);

    // Luodaan siemenluvulle etiketti ja valintalaatikko.
    // Määritetään niiden sijainti ja koko ruudulla.
    seed_label_ = new QLabel(SEED_LABEL_TEXT, this);
    seed_label_->setGeometry(MARGIN, 2*MARGIN,
                             DEFAULT_LABEL_WIDTH,
                             DEFAULT_WIDGET_HEIGHT);

    seed_spinbox_ = new QSpinBox(this);
    seed_spinbox_->setGeometry(2*MARGIN, 2*MARGIN,
                               2*DEFAULT_WIDGET_WIDTH,
                               DEFAULT_WIDGET_HEIGHT);

    // Määritetään valintalaatikon suurin ja pienin arvo.
    seed_spinbox_->setMinimum(0);
    seed_spinbox_->setMaximum(999);

    // Luodaan tavoiteluvulle etiketti ja valintalaatikko.
    // Määritetään niiden sijainti ja koko ruudulla.
    goal_value_label_ = new QLabel(GOAL_VALUE_TEXT, this);
    goal_value_label_->setGeometry(MARGIN, 2.5*MARGIN,
                                   DEFAULT_LABEL_WIDTH,
                                   DEFAULT_WIDGET_HEIGHT);

    goal_value_spinbox_ = new QSpinBox(this);
    goal_value_spinbox_->setGeometry(2*MARGIN, 2.5*MARGIN,
                                     2*DEFAULT_WIDGET_WIDTH,
                                     DEFAULT_WIDGET_HEIGHT);

    // Asetetaan kummankin valintalaatikon taustaväri valkoiseksi.
    seed_spinbox_->setStyleSheet(WHITE_BACKGROUND);
    goal_value_spinbox_->setStyleSheet(WHITE_BACKGROUND);

    // Määritetään valintalaatikon suurin ja pienin arvo sekä oletusarvo.
    goal_value_spinbox_->setMinimum(4);
    goal_value_spinbox_->setMaximum(pow(2,board_size_*board_size_));
    goal_value_spinbox_->setValue(DEFAULT_GOAL);

    // Luodaan 'Valmis'-painonappi ja
    // määritetään sen sijainti ja koko ruudulla.
    ready_button_ = new QPushButton(READY_BUTTON_TEXT, this);
    ready_button_->setGeometry(2.7*MARGIN, 3*MARGIN,
                               5*DEFAULT_WIDGET_WIDTH,
                               DEFAULT_WIDGET_HEIGHT);

    // Asetetaan napin taustaväri valkoiseksi.
    ready_button_->setStyleSheet(WHITE_BACKGROUND);

    // Yhdistetään painonappia klikattaessa lähetettävä signaali
    // vastaanottajaan, joka sulkee aloitusruudun.
    connect(ready_button_, &QPushButton::clicked,
            this, &MainWindow::handle_ready_button_click);
}

void MainWindow::update_gameboard()
{ 
    // Luodaan "maisema" ja "näkymä" vain metodin
    // ensimmäisellä kutsumakerralla.
    if(!gameboard_created_)
    {
        // Luodaan widgetit itse pelilautaa varten.
        scene_ = new QGraphicsScene(this);
        graphics_view_ = new QGraphicsView(scene_, this);

        // Määritetään pelilaudan sijainti ja koko ruudulla.
        // Pelilaudan koko lasketaan <SIZE>-vakion avulla.
        graphics_view_->setGeometry(RECT_COORD, RECT_COORD,
                                    board_size_*TILE_BASE_SIDE_LENGTH,
                                    board_size_*TILE_BASE_SIDE_LENGTH);
        graphics_view_->setStyleSheet(WHITE_BACKGROUND);

        // Asetetaan "maisemassa" esitettävä "näkymä",
        // ja määritetään sen koko ja sijainti.
        graphics_view_->setScene(scene_);
        scene_->setSceneRect(RECT_COORD, RECT_COORD,
                             board_size_*TILE_SIDE_LENGTH,
                             board_size_*TILE_SIDE_LENGTH);

        // Alustetaan "maisema" piilotetuksi eli pois ruudulta.
        graphics_view_->setVisible(false);

        // Luodaan nappi, jolla voidaan aloittaa peli alusta. Lisäksi
        // määritetään sen koko ja sijainti.
        reset_midgame_button_ = new QPushButton(RESET_BUTTON_TEXT, this);
        reset_midgame_button_->setGeometry(MARGIN, (board_size_+1)*MARGIN,
                                           4*DEFAULT_WIDGET_WIDTH,
                                           DEFAULT_WIDGET_HEIGHT);

        // Asetetaan napin taustaväri valkoiseksi.
        reset_midgame_button_->setStyleSheet(WHITE_BACKGROUND);

        // Yhdistetään napin lähettämä signaali vastaanottajaan.
        connect(reset_midgame_button_, &QPushButton::clicked,
                this, &MainWindow::handle_reset_midgame_button_click);

        // Alustetaan nappi piilotettavaksi.
        reset_midgame_button_->setVisible(false);

        // Vaihdetaan lipun arvo, jolloin edellä luotuja
        // widgettejä ei luoda uudelleen pelin suorituksen aikana.
        gameboard_created_ = true;
    }
    // Käydään pelilaudan jokainen x-koordinaatti yksitellen läpi.
    for(int i = 0; i < board_size_; i++)
    {
        // Käydään kussakin x-koordinaatissa eli sarakkeessa
        // sijaitsevat y-koordinaatit läpi yksitellen.
        for(int j = 0; j < board_size_; j++)
        {
            // Luodaan x- ja y-koordinaateista pari, jonka avulla
            // voidaan hakea koordinaateissa sijaitseva pelilaudan laatta.
            std::pair<int, int> coords = std::make_pair(j, i);

            // Haetaan GameBoard-luokan pelilautaoliosta
            // koordinaateissa sijaitseva NumberTile-luokan laattaolio.
            tile_ = board_.get_item(coords);

            // Haetaan laatan arvo olion metodin avulla.
            int tile_value = tile_->get_value();

            // Testitulosteet laatan arvolle ja koordinaateille.
            //qDebug() << "X:Y" << coords.second << ":" << coords.first;
            //qDebug() << "VALUE:" << tile_value;

            // Muunnetaan haettu kokonaislukuarvo QString-muotoon.
            QString tile_value_str = QString::number(tile_value);

            // Luodaan widgetti pelilaudan laatalle. Määritetään sen
            // sijainti ja koko pelilaudalla. Lisätään lopuksi luotu
            // laattawidgetti "näkymään".
            rect_ = new QGraphicsRectItem(RECT_COORD, RECT_COORD,
                                          TILE_SIDE_LENGTH, TILE_SIDE_LENGTH);
            rect_->setPos(coords.second*TILE_SIDE_LENGTH,
                          coords.first*TILE_SIDE_LENGTH);
            scene_->addItem(rect_);

            // Lisätään tekstikenttä ja vaihdetaan laatan
            // väri vain jos laatan arvo ei ole nolla.
            if(tile_value != 0)
            {
                // Vaihdetaan laatan väri sen arvon mukaan.
                change_tile_color(tile_value);

                // Luodaan tekstikenttä, jossa laatan arvo esitetään.
                // Määritetään laattawidgetti tekstikentän vanhemmaksi.
                tile_text_ = new QGraphicsTextItem(tile_value_str, rect_);
                tile_text_->setParentItem(rect_);
            }
        }
    }
}

void MainWindow::react_to_win()
{
    // Piilotetaan pelilauta ja reset-nappi, koska peli on päättynyt.
    graphics_view_->setVisible(false);
    reset_midgame_button_->setVisible(false);

    // Piilotetaan myös pelaajan pelinaikaiset pisteet kertova etiketti
    // sekä ajastimeen kuuluvat komponentit.
    player_score_label_->setVisible(false);
    lcd_min_->setVisible(false);
    min_label_->setVisible(false);
    lcd_sec_->setVisible(false);
    sec_label_->setVisible(false);

    // Värjätään ruudun tausta vihreäksi merkkinä voitosta.
    this->setStyleSheet(GREEN_BACKGROUND);

    // Kutsutaan metodia, joka näyttää pelaajalle viestilaatikon.
    show_end_screen_message_box();
}

void MainWindow::react_to_lose()
{
    // Piilotetaan pelilauta ja reset-nappi, koska peli on päättynyt.
    graphics_view_->setVisible(false);
    reset_midgame_button_->setVisible(false);

    // Piilotetaan myös pelaajan pelinaikaiset pisteet kertova etiketti
    // sekä ajastimeen kuuluvat komponentit.
    player_score_label_->setVisible(false);
    lcd_min_->setVisible(false);
    min_label_->setVisible(false);
    lcd_sec_->setVisible(false);
    sec_label_->setVisible(false);

    // Värjätään ruudun tausta punaiseksi merkkinä häviöstä.
    this->setStyleSheet(RED_BACKGROUND);

    // Kutsutaan metodia, joka näyttää pelaajalle viestilaatikon.
    show_end_screen_message_box(true);
}

void MainWindow::show_end_screen_message_box(bool has_lost)
{
    // Muuttuja, jonne tallennetaan pelaajan
    // valinta viestilaatikon vaihtoehdoista.
    int message_box_result = 0;

    // Riippumatta siitä, voittiko vai hävisikö pelaaja, luodaan
    // viestilaatikko, jossa kysytään pelaajan halukkuutta jatkaa pelaamista.
    // Reagoidaan päätökseen pelaajan valinnan mukaisesti.

    // Viestilaatikko, jos pelaaja voitti.
    if(!has_lost)
    {
        message_box_result = QMessageBox::question(nullptr,
                                                   WIN_TITLE_TEXT,
                                                   PLAY_AGAIN_TEXT,
                                                   QMessageBox::Yes,
                                                   QMessageBox::No);
    }
    // Viestilaatikko, jos pelaaja hävisi.
    else
    {
        message_box_result = QMessageBox::question(nullptr,
                                                   LOSE_TITLE_TEXT,
                                                   TRY_AGAIN_TEXT,
                                                   QMessageBox::Yes,
                                                   QMessageBox::No);
    }

    // Jos pelaaja haluaa jatkaa pelaamista, lähetetään reset-signaali.
    if(message_box_result == QMessageBox::Yes)
    {
        emit reset_game();
    }
    // Jos pelaaja puolestaan haluaa lopettaa pelaamisen,
    // lähetetään signaali, joka päättää pelin.
    else
    {
        emit quit_game();
    }
}

void MainWindow::change_tile_color(int value)
{
    // Tarkistetaan, onko laatan arvo suurempi kuin
    // pelin oletusarvoinen tavoiteluku, eli 2048.
    if(value > DEFAULT_GOAL)
    {
        // Haetaan tietorakenteesta viimeinen avain,
        // eli tässä tapauksessa 4096.
        value = TILES_RGB.end()->first;
    }

    // Haetaan tietorakenteesta lukuarvoa vastaavasta
    // listasta sen alkioina olevat RGB-väriarvot.

    // R, eli punainen väriarvo.
    int red = TILES_RGB.at(value).at(0);

    // G, eli vihreä väriarvo.
    int green = TILES_RGB.at(value).at(1);

    // B, eli sininen väriarvo.
    int blue = TILES_RGB.at(value).at(2);

    // Hyödynnetään <QColor> -luokkaa, ja asetetaan
    // värimuuttujaksi RGB-arvoja vastaava väri.
    QColor color = QColor(red, green, blue);

    // Luodaan <QBrush> -luokan ja juuri asetetun värin
    // avulla pensseli, jota käytetään laatan värjäämiseen.
    QBrush brush = QBrush(color);

    // Maalataan laatta luodulla pensselillä.
    rect_->setBrush(brush);
}

void MainWindow::init_board_size_widgets()
{
    // Luodaan uusi <QSlider>-luokan liukusäädinolio.
    size_slider_ = new QSlider(Qt::Horizontal, this);

    // Asetetaan suurin ja pienin arvo sekä
    // oletusarvo ja arvojen välinen askel.
    size_slider_->setMinimum(3);
    size_slider_->setMaximum(8);
    size_slider_->setTickInterval(1);
    size_slider_->setValue(DEFAULT_BOARD_SIZE);

    // Määritetään askeleet näkyviin säätimen kummallekin puolelle.
    size_slider_->setTickPosition(QSlider::TicksBothSides);

    // Määritetään liukusäätimen taustaväri
    // sekä koko ja sijainti alkuruudulla.
    size_slider_->setStyleSheet(WHITE_BACKGROUND);
    size_slider_->setGeometry(4*MARGIN, 2.5*MARGIN,
                              6*DEFAULT_WIDGET_WIDTH, DEFAULT_WIDGET_HEIGHT);

    // Luodaan uusi <QLabel>-luokan etikettiolio ja määritetään
    // sen sijainti ja koko aloitusruudulla.
    size_label_ = new QLabel(SIZE_LABEL_TEXT, this);
    size_label_->setGeometry(4.2*MARGIN, 2*MARGIN,
                             0.75*DEFAULT_LABEL_WIDTH, DEFAULT_WIDGET_HEIGHT);

    // Tasataan etiketin teksti keskelle sekä vaaka- että pystysuunnassa.
    size_label_->setAlignment(Qt::AlignHCenter);
    size_label_->setAlignment(Qt::AlignCenter);

    // Luodaan uusi <QLabel>-luokan etikettiolio ja määritetään
    // sen sijainti ja koko aloitusruudulla.
    size_value_label_ = new QLabel(this);
    size_value_label_->setGeometry(6.1*MARGIN, 2*MARGIN,
                                   1.5*DEFAULT_WIDGET_WIDTH,
                                   DEFAULT_WIDGET_HEIGHT);

    // Asetetaan etiketin oletusteksti ja tasataan se kummassakin suunnassa.
    size_value_label_->setText(DEFAULT_SIZE_VALUE_TEXT);
    size_value_label_->setAlignment(Qt::AlignHCenter);
    size_value_label_->setAlignment(Qt::AlignCenter);

    // Yhdistetään liukusäätimen liikutus yksityiseen slottiin.
    connect(size_slider_, &QSlider::valueChanged, this,
            &MainWindow::handle_size_slider_movement);
}

void MainWindow::init_player_score_widgets()
{
    // Muotoillaan pelaajan pisteistä ja
    // loppuliitteestä esitys pelaajan alkupisteistä.
    QString score = QString::number(current_player_score_) + POINTS_SUFFIX;

    // Luodaan uusi etiketti, jonne asetetaan muotoiltu <QString>-olio.
    player_score_label_ = new QLabel(score, this);

    // Määritetään etiketin taustaväri sekä sijainti ja koko ruudulla.
    player_score_label_->setStyleSheet(WHITE_BACKGROUND);
    player_score_label_->setGeometry((board_size_+0.1)*MARGIN,
                                     (board_size_+0.1)*MARGIN,
                                     MARGIN,
                                     DEFAULT_WIDGET_HEIGHT);

    // Muotoillaan etiketin sisältö keskelle kummassakin suunnassa.
    player_score_label_->setAlignment(Qt::AlignHCenter);
    player_score_label_->setAlignment(Qt::AlignCenter);

    // Asetetaan etiketti näkyviin.
    player_score_label_->setVisible(true);
}

void MainWindow::init_timer_widgets()
{
    // Luodaan uusi <QLCDNumber>-luokan numeroruutuolio.
    // Määritetään sen taustaväri sekä sijainti ja koko ruudulla.
    lcd_min_ = new QLCDNumber(this);
    lcd_min_->setGeometry(0.5*MARGIN,
                          (board_size_+0.1)*MARGIN,
                          DEFAULT_WIDGET_WIDTH,
                          DEFAULT_WIDGET_HEIGHT);
    lcd_min_->setStyleSheet(WHITE_BACKGROUND);

    // Asetetaan numeroruutu näkyviin ja määritetään numeroiden tyyli
    // <QLCDNumber::Flat>-vakiolla, jolloin numeroiden todellinen
    // väri (musta) saadaan näkyviin.
    lcd_min_->setVisible(true);
    lcd_min_->setSegmentStyle(QLCDNumber::Flat);

    // Luodaan uusi <QLabel>-luokan etikettiolio, johon asetetaan
    // teksti minuutteja varten. Määritetään etiketin sijainti ja koko
    // ruudulla, tasataan teksti keskelle kummassakin suunnassa ja
    // määritetään se näkyviin ruudulle.
    min_label_ = new QLabel(MINUTES_TEXT, this);
    min_label_->setGeometry(1*MARGIN,
                            (board_size_+0.1)*MARGIN,
                            1.5*DEFAULT_WIDGET_WIDTH,
                            DEFAULT_WIDGET_HEIGHT);
    min_label_->setAlignment(Qt::AlignHCenter);
    min_label_->setAlignment(Qt::AlignCenter);
    min_label_->setVisible(true);

    // Luodaan uusi <QLCDNumber>-luokan numeroruutuolio.
    // Määritetään sen taustaväri sekä sijainti ja koko ruudulla.
    lcd_sec_ = new QLCDNumber(this);
    lcd_sec_->setGeometry(2*MARGIN,
                          (board_size_+0.1)*MARGIN,
                          DEFAULT_WIDGET_WIDTH,
                          DEFAULT_WIDGET_HEIGHT);
    lcd_sec_->setStyleSheet(WHITE_BACKGROUND);

    // Asetetaan numeroruutu näkyviin ja määritetään numeroiden tyyli.
    lcd_sec_->setVisible(true);
    lcd_sec_->setSegmentStyle(QLCDNumber::Flat);

    // Luodaan uusi <QLabel>-luokan etikettiolio, johon asetetaan
    // teksti sekunteja varten. Määritetään etiketin sijainti ja koko
    // ruudulla, tasataan teksti keskelle kummassakin suunnassa ja
    // määritetään se näkyviin ruudulle.
    sec_label_ = new QLabel(SECONDS_TEXT, this);
    sec_label_->setGeometry(2.5*MARGIN,
                            (board_size_+0.1)*MARGIN,
                            1.5*DEFAULT_WIDGET_WIDTH,
                            DEFAULT_WIDGET_HEIGHT);
    sec_label_->setAlignment(Qt::AlignHCenter);
    sec_label_->setAlignment(Qt::AlignCenter);
    sec_label_->setVisible(true);
}

