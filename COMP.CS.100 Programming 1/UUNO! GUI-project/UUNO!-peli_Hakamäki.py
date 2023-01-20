"""
COMP.CS.100 Ohjelmointi 1 / Programming 1
Student Id: *******
Name:       Saku Hakamäki
Email:      *********************

Ohjelmointi 1 -kurssin viimeisenä projektina tein UNO! -korttipelistä
vaikutteita ottaneen version, UUNO! -pelin. Pelin säännöt ovat hyvin
samankaltaiset alkuperäiseen peliin verrattuna. Peli aukeaa päävalikkoon,
josta käyttäjä voi asettaa peliin osallistuvien pelaajien lukumäärän.
Peli voidaan sulkea "Quit game" -napista. "Start game" -nappia
painettaessa syötekenttään asetetun arvon kelvollisuus tarkastetaan.
Jos arvot, eivät kelpaa, kerrotaan siihen syy päävalikon alalaidassa.

Jos arvot sen sijaan ovat kelvollisia, peli alkaa ja siirrytään itse
peliruutuun. Peliruudulla näkyy syötetty määrä pelaajia kummallakin
puolella pöytää, keskellä nostopakka ja värinvalintanapit sekä
lyöntipakka, jonne pöydälle asetetut kortit päätyvät. Pelaaja 1 aloittaa
pelin painamalla yhtä kädessään olevista korttinapeista. Siirrytään
seuraavaan pelaajaan, joka voi painaa korttinapeista vain lyötäväksi
sopivia kortteja. Pöytään lyötäviksi sopivat ne kortit, jotka ovat
samaa väriä (punainen / sininen / keltainen / vihreä / villi kortti)
tai tyyppiä (numero- / +2- / villit kortit). Villit kortit voi lyödä
minkä tahansa muun kortin päälle. Jos pelaaja ei voi lyödä kädestään
korttia, on pelaajan ainoa mahdollisuus nostaa korttinappi nostopakasta.
Kortin nostettuaan vuoro siirtyy seuraavaan pelaajaan.

Ehkä suurimpana erona alkuperäisiin sääntöihin on sääntö, jonka mukaan
pelaajan lyödessä villin kortin eli +4/värinvaihto- tai värinvaihtokortin,
ja seuraavana vuorossa oleva pelaaja ei voi lyödä tähän mitään korttia
vaan joutuu nostamaan pakasta kortin, niin tämän jälkeen pöytään lyödyn
villin kortin päälle saa lyödä vain villin kortin. Tämän uuden villin
kortin lyönyt pelaaja saa jälleen valita seuraavaksi lyötävän kortin värin.
Jos seuraavana vuorossa oleva pelaaja ei voi lyödä tähän korttia, toimitaan
samoin kuin edellä. Sääntö toimii eräänlaisena rangaistuksena siitä,
että pelaajan on aina pakko lyödä kädestään sopiva kortti pöytään, jolloin
bluffaamisen mahdollisuus poistuu.

Pelaajan lyödessä +2-kortti, seuraava pelaaja joutuu nostamaan kaksi
korttia nostopakasta. Jos pelaaja haluaa lyödä värinvaihtokortin, tulee
peliruudulla haluttua seuraavaa sallittua väriä vastaavaa värinappia
painaa ENNEN VÄRINVAIHTOKORTIN LYÖMISTÄ PÖYTÄÄN. Muussa tapauksessa
pelaaja on unohtanut valita värin ja se valitaan satunnaisesti.
+4 / värinvaihtokortin lyödessään pelaaja voi vaikuttaa seuraavaan
sallittuun väriin, jonka lisäksi seuraava pelaaja joutuu nostamaan
pakasta neljä korttia.

Vuorot vaihtuvat ja peli kulkee tähän malliin kunnes jokin pelaajista
lyö pöytään viimeisen kyseisen pelaaja kädessä olleen kortin. Tämä
pelaaja on voittanut pelin. Peli päättyy, siirrytään voittoruudulle,
jossa onnitellaan voittanutta pelaajaa ja näytetään voittoanimaatio.
Peli voidaan sulkea voittoruudulla olevasta aiemmin päävalikossa
sijainneesta "Quit game" -napista.


Pelissä on käytetty Frame-käyttöliittymäkomponenttia, johon tutustuttu
itsenäisesti. Tämän lisäksi ohjelma sisältää suuren määrän samanlaisia
komponentteja, joita luodaan silmukassa. Näitä komponentteja tässä
ohjelmassa pelikorttinapit sekä pelaajaolioiden ja pakkaolioiden
kehyskomponentit, jotka kummatkin tallennetaan omiin sanakirjoihinsa
myöhempää käyttöä varten. Ohjelman toteutuksessa on hyödynnetty tkinter-
sekä random -standardikirjastoja.

Mielestäni UUNO! -peli toteuttaa kehittyneelle
käyttöliittymälle asetetut vaatimukset.
"""

# Otetaan käyttöön tkinter -kirjasto, sekä
# random -kirjastosta metodit shuffle ja randint.
from tkinter import *
from random import shuffle, randint


class GraphicalUserInterface:
    """
    Luokka toteuttaa UUNO! -pelin käyttöliittymän sekä muut
    käyttöliittymään liittyvät toiminnot tkinter -kirjastoa hyödyntäen.
    """

    def __init__(self):
        """
        Rakentaja, luo useita pelin aikana tarvittavia
        käyttöliittymäkomponentteja, kuten nappeja ja tekstikenttiä.
        Lisäksi lataa joitakin pelin aikana tarvittavia kuvatiedostoja
        ja alustaa muita pelin aikana tarvittavia attribuutteja.
        """

        # Luodaan pääikkuna ja muutetaan sen otsikko sopivaksi.
        self.__main_window = Tk()
        self.__main_window.title("UUNO!")

        # Tallennetaan attribuuttiin päävalikon kuva.
        self.__uunologo_kuva = PhotoImage(file="uuno_logo.png")

        # Luodaan päävalikossa tarvittavat napit.
        self.__start_button = Button(self.__main_window, text="Start game",
                                     command=self.participant_namelist)
        self.__quit_button = Button(self.__main_window, text="Quit game",
                                    command=self.close_window)

        # Luodaan tekstikenttä, johon päävalikon kuva tallennetaan.
        self.__title_label = Label(self.__main_window,
                                   image=self.__uunologo_kuva)

        # Teksti- ja syöttökenttä pelaajien lukumäärän asettamista varten.
        self.__player_label = Label(self.__main_window,
                                    text="Players (2 - 10)")
        self.__player_value = Entry()

        # Tekstikenttä, joka toimii päävalikon inforuutuna.
        self.__info_label = Label(self.__main_window,
                                  text="Greetings, player! \n Welcome to play "
                                       "UUNO!")

        # Asetetaan päävalikon osat näkyviin näytölle.
        self.__title_label.grid(row=1, column=0, rowspan=4, columnspan=5)
        self.__info_label.grid(row=7, column=0, rowspan=2, columnspan=5)
        self.__player_label.grid(row=5, column=2)
        self.__player_value.grid(row=6, column=2)
        self.__start_button.grid(row=1, column=5, rowspan=5,
                                 columnspan=3, sticky=NW+SE)
        self.__quit_button.grid(row=6, column=5, rowspan=3,
                                columnspan=3, sticky=NW+SE)

        # Seuraavaksi luodaan komponentteja,
        # jotka ovat näkyvissä itse pelin aikana.

        # Luodaan frame -komponentti, jonka sisälllä olevista napeista pelaaja
        # voi pelin aikana valita haluamansa värin lyödessään "villin" kortin.
        self.__change_color_frame = Frame(self.__main_window,
                                          highlightbackground="black",
                                          highlightthickness=3)

        # Komponentin "nimilappu".
        frame_nametag = Label(self.__change_color_frame,
                              text="Color Selection")
        frame_nametag.grid(row=1, column=0, columnspan=2, sticky=EW)

        # Luodaan framen sisällä olevat napit,
        # määritetään nappien isännäksi edellä luotu frame-komponentti.

        # Punainen värinvalintanappi.
        self.__red_color_button = Button(self.__change_color_frame,
                                         background="red", borderwidth=2,
                                         relief=RAISED)

        # Kytketään napinpainalluksen jälkeen suoritettava metodi nappiin.
        self.__red_color_button.bind("<ButtonRelease>",
                                     self.set_color_parameter)

        # Tallennetaan nappiin värinvalintaparametrin arvo.
        self.__red_color_button.selected_color = "red"

        # Sininen värinvalintanappi.
        self.__blue_color_button = Button(self.__change_color_frame,
                                          background="blue", borderwidth=2,
                                          relief=RAISED)

        # Napinpainalluksen jälkeen suoritettava metodi.
        self.__blue_color_button.bind("<ButtonRelease>",
                                      self.set_color_parameter)

        # Värinvalintaparametrin arvo.
        self.__blue_color_button.selected_color = "blue"

        # Keltainen värinvalintanappi.
        self.__yellow_color_button = Button(self.__change_color_frame,
                                            background="yellow", borderwidth=2,
                                            relief=RAISED)

        # Napinpainalluksen jälkeen suoritettava metodi.
        self.__yellow_color_button.bind("<ButtonRelease>",
                                        self.set_color_parameter)

        # Värinvalintaparametrin arvo.
        self.__yellow_color_button.selected_color = "yellow"

        # Vihreä värinvalintanappi.
        self.__green_color_button = Button(self.__change_color_frame,
                                           background="green", borderwidth=2,
                                           relief=RAISED)

        # Napinpainalluksen jälkeen suoritettava metodi.
        self.__green_color_button.bind("<ButtonRelease>",
                                       self.set_color_parameter)

        # Värinvalintaparametrin arvo.
        self.__green_color_button.selected_color = "green"

        # Sijoitetaan napit paikoilleen värinvalinta -frameen.
        self.__red_color_button.grid(row=2, column=0, sticky=EW)
        self.__blue_color_button.grid(row=2, column=1, sticky=EW)
        self.__yellow_color_button.grid(row=3, column=0, sticky=EW)
        self.__green_color_button.grid(row=3, column=1, sticky=EW)

        # Luodaan attribuutti, jonka arvoa värinvalintanapit kontrolloivat.
        self.__selected_color_parameter = ""

        # Tekstikenttä, jossa pelin aikana ilmoitetaan vuorossa oleva pelaaja.
        self.__player_in_turn_label = Label(self.__main_window, text="",
                                            background="black",
                                            foreground="white")

        # Tekstikenttä, ilmoittaa valitun värin villin kortin lyömisen jälkeen.
        self.__chosen_color_label = Label(self.__main_window, text="",
                                          background="white",
                                          foreground="black")

        # Tallennetaan attribuuttiin kortin takakannen kuvatiedosto.
        self.__card_image_back = PhotoImage(file="card_back.png")

        # Alustetaan vielä muita pelin aikana tarvittavia attribuutteja.

        # Luodaan korttien nimille lista ja alustetaan pakka.
        self.__deck_list = []

        # Metodi tallentaa edellä luotuun listaan korttien nimet
        # satunnaisessa järjestyksessä eli korttipakka sekoitetaan.
        self.initialize_deck()

        # Lista, jonne tallennetaan sopivien korttien nimet, eli
        # toisinsanoen kortit, jotka pelaajan on mahdollista lyödä pöytään.
        self.__suitable_cards = []

        # Lista, jonne tallennetaan peliin osallistuvien pelaajien nimet.
        self.__participant_names = []

        # Sanakirja, jossa avaimena kortin nimi ja
        # hyötykuormana pelin aikana käytetyt korttinapit.
        self.__deck_library = {}

        # Sanakirja, jossa avaimena pelaajan nimi ja hyötykuormana
        # frame -komponentti, jossa sisällä pelaajan kädessä olevat kortit.
        self.__frame_library = {}

        # Luodaan lista, jossa alkioina Participant -luokkaan
        # kuuluvat lyönti- ja nostopakkaoliot.
        self.__deck_objects = []

        # Lista, jossa alkioina Participant -luokkaan kuuluvat pelaajaoliot.
        self.__participant_objects = []

        # Luodaan attribuutit, joihin tallentuu
        # korttinappeihin tallennetut arvot.
        self.__frame_id = None
        self.__card_id = None

        # Atrribuutti, jonka avulla määritetään vuorossa oleva pelaaja.
        self.__in_turn_index = 0

        # Attribuutti, joka määrittää pelivuoron numeron.
        self.__round_index = 1

        # Lopuksi vielä muutama voittoruudussa tarvittava attribuutti.

        # Ladataan voittoruudussa tarvittavat kuvatiedostot
        # ja tallennetaan ne listan alkioiksi.
        self.__prize_photos = []
        for filename in ["uuno_prize_1.png", "uuno_prize_2.png",
                         "uuno_prize_3.png"]:
            self.__prize_photos.append(PhotoImage(file=filename))

        # Laskuri, jolla määritetään voittoruudun animaation päivitystiheys.
        self.__refresh_counter = 0

        # Tekstikenttä, jossa animaatio esitetään.
        self.__prize_animation_label = Label(self.__main_window)

        # Attribuutti, jonne tallennetaan voittaneen pelaajan nimi.
        self.__winner = ""

        # Tekstikenttä, jossa onnitellaan voittajaa voittoruudussa.
        self.__congratulations_label = Label(self.__main_window, text="",
                                             background="black",
                                             foreground="white")

    def mr_referee(self):
        """
        Metodi käsittelee kaikki yhden pelivuoron aikana tapahtuvat
        toiminnot, ja on täten yksi tärkeimmistä osista ohjelman
        toiminnan kannalta. Metodia kutsutaan ensimmäisen kerran
        pelipöydän valmistelujen yhteydessä. Tämän jälkeen metodia
        kutsutaan joka kerta nappia painettaessa.
        """

        # Ensimmäinen ehto toteutuu vain pelin käynnistyessä. Ehdon
        # toteutuessa alustetaan joitakin komponentteja sopivaan tilaan.
        if self.__round_index == 1:

            # Vuorossa olevan pelaajan nimi merkkijonona.
            in_turn = self.get_in_turn()

            # Haetaan tallennettua nimeä vastaava
            # Participant -luokan pelaajaolio.
            participant_in_turn = self.get_participant(in_turn)

            # Vuorossa olevan pelaajan korteista ne,
            # jotka sopivat lyötäväksi pöytään.
            self.__suitable_cards = self.suitable_cards(participant_in_turn,
                                                        first_round=True)

            # Käännetään ensimmäisenä vuorossa olevan pelaajan
            # kortit näkyviin ja piilotetaan muiden pelaajien kortit.
            self.flip_cards(participant_in_turn)

            # Ensimmäisenä vuorossa olevan pelaajan sopivat kortit
            # NORMAL -tilaan, kaikki muut kortit DISABLED -tilaan.
            self.update_cards_state()

            # Koska pelaaja voi lyödä kädestään kortin, pakasta nosto estetään.
            take_decks_deck = self.__deck_objects[0].get_deck()

            for card_name in take_decks_deck:
                self.__deck_library[card_name].configure(state=DISABLED)

            # Lisätään kierrosindeksiä yhdellä,
            # jolloin tämä ehto ei täyty enää toista kertaa.
            self.__round_index += 1

            # Alkuvalmisteluiden vuoksi vuorossa olevan ilmoittama indeksi
            # on kasvanut yhdellä, alustetaan se uudelleen alkuarvoonsa.
            self.__in_turn_index = 0

            # Ilmoitetaan ensimmäisenä vuorossa oleva
            # pelaaja ja pyydetään tätä aloittamaan peli.
            self.__player_in_turn_label.configure(text=f"It's {in_turn}'s "
                                                       f"turn. Please start "
                                                       f"the game by placing a"
                                                       f" card on the table.")

            # Muokataan myös valitun värin ilmoittavan tekstikentän tekstiä.
            self.__chosen_color_label.configure(text="No wild card has been "
                                                     "placed on the table "
                                                     "for now.")

        # Koska ensimmäinen ehto ei alkuvalmisteluiden
        # jälkeen voi enää täyttyä, kaikkien muiden vuorojen
        # aikana toiminnot keskittyvät tämän ehdon alle.
        else:

            # Tutkitaan, onko pelaajan painamaa korttinappia sallittua painaa.
            # Jos painetun napin klikkaaminen ei ole sallittua, joutuu pelaaja
            # pettymyksekseen tyytymään jonkin toisen napin painamiseen.

            # Aloitetaan tutkiminen, jos painetun napin nimitunniste
            # ei ole aiemmin todettu sopivan pöytään lyötäväksi.
            # Muussa tapauksessa kortti hyväksytään heti ja suoritus jatkuu.
            if self.__card_id not in self.__suitable_cards:

                # Tutkinta jatkuu, jos painettu nappi kuuluukin nostopakkaan.
                if self.__card_id in self.__deck_objects[0].get_deck():

                    # Jos pelaaja voi lyödä kädestään kortin,
                    # ei kortin nostaminen pakasta ole sallittua.
                    # Tämän seurauksena painettu korttinappi tulee hylätyksi.
                    if len(self.__suitable_cards) != 0:
                        return

                    # Nostopakan kortin painaminen hyväksytään, jos ja
                    # vain jos pelaaja ei voi lyödä korttia kädestään.
                    else:
                        pass

                # Myöskään muiden pelaajien korttien tutkiminen
                # tai jo pöytään lyötyjen korttien kääntely ei ole
                # sallittua, joten painettu korttinappi tulee hylätyksi.
                else:
                    return

            # Muokataan tekstikentän teksti ja väri,
            # jos lyöty kortti ei olekaan villi kortti.
            self.__chosen_color_label.configure(text="There is no wild card "
                                                     "on top of the Place-"
                                                     "Deck.")
            self.__chosen_color_label.configure(background="white")

            # Vuorossa olevan pelaajan nimi merkkijonona.
            in_turn = self.get_in_turn()

            # Haetaan tallennettua nimeä vastaava
            # Participant -luokan pelaajaolio.
            participant_in_turn = self.get_participant(in_turn)

            # Seuraavana vuorossa olevan pelaajan nimi merkkijonona.
            next_in_turn = self.get_next_in_turn()

            # Haetaan seuraavana vuorossa olevan nimeä
            # vastaava Participant -luokan pelaajaolio.
            next_in_turn_object = self.get_participant(next_in_turn)

            # Ilmoitetaan vuorossa oleva pelaaja, ja pyydetään tätä
            # jatkamaan peliä lyömällä pöytään kortti jos vain mahdollista.
            self.__player_in_turn_label.configure(text=f"It's {next_in_turn}'s"
                                                       f" turn. Please place a"
                                                       f" card on the table if"
                                                       f" possible.")

            # Siirrytään lyötyyn korttiin reagoivaan metodiin, jossa
            # suoritetaan erilaisia toimenpiteitä lyödystä kortista riippuen.
            self.make_card_decisions(in_turn, participant_in_turn,
                                     next_in_turn_object)

            # Seuraavana toiminnot, kun lyötyyn korttiin on reagoitu.

            # Käännetään seuraavana vuorossa olevan pelaajan
            # kortit näkyviin ja piilotetaan muiden pelaajien kortit.
            self.flip_cards(next_in_turn_object)

            # Kaikkien muiden paitsi seuraavana vuorossa
            # olevan pelaajan kortit DISABLED -tilaan.
            self.update_cards_state()

            # Seuraavana vuorossa olevan pelaajan
            # sopivat kortit NORMAL -tilaan.
            self.update_cards_state(turn_end=True)

            # Jos pelaaja voi lyödä kädestään kortin, pakasta nosto estetään.
            if len(self.__suitable_cards) != 0:

                # Nostopakassa olevat kortit.
                take_decks_deck = self.__deck_objects[0].get_deck()

                # Asetetaan nostopakan kortit yksitellen DISABLED -tilaan.
                for card_name in take_decks_deck:
                    self.__deck_library[card_name].configure(state=DISABLED)

            # Jos vuorossa olevan pelaajan kädestä
            # loppuvat kortit, on pelaaja voittanut.
            if len(participant_in_turn.get_deck()) == 0:

                # Tallennetaan vuorossa ollut pelaaja voittajaksi.
                self.__winner = f"{in_turn}"

                # Siirrytään voittoruutuun.
                self.show_win_screen()

            # Jos nostopakassa on enintään 4 korttia, siirretään
            # lyöntipakasta kortit nostopakkaan ja sekoitetaan nostopakka.
            if len(self.__deck_objects[0].get_deck()) <= 4:

                # Lyöntipakassa olevat kortit.
                place_decks_deck = self.__deck_objects[1].get_deck()

                # Tehdään toistoja, kunnes kaikki muut paitsi
                # lyöntipakan päällimmäinen kortti on siirretty nostopakkaan.
                for repetition in range(0, len(place_decks_deck) - 1):

                    # Siirrettävä kortti on lyöntipakan pohjalla.
                    card_to_transfer = place_decks_deck[0]

                    # Siirretään kortti pakasta toiseen.
                    transfer_card(card_to_transfer, "Take-Deck",
                                  participant_in_turn, self.__deck_objects,
                                  reset_decks=True)

                # Kun siirrot on suoritettu, sekoitetaan nostopakassa
                # olevat kortit Participant -luokan metodin avulla.
                self.__deck_objects[0].shuffle_deck()

    def initialize_deck(self):
        """
        Metodi luo kutsuttaessa korttien nimet ja tallentaa ne
        attribuutti-listan alkioiksi. Lopuksi listaan tallennetut
        korttien nimet sekoitetaan shuffle -metodin avulla.
        """

        # Listan alkioina pelikorttien kaikki mahdolliset värit.
        colours = ["red", "yellow", "blue", "green"]

        # Listan alkioina kaikki eri korttityypit.
        # Jokaisessa värissä on yksi 0 -kortti, kaksi korttia
        # jokaisesta numerosta välillä 1-9 ja kaksi +2 -korttia.
        card_numbers = ["0", "1", "1", "2", "2", "3", "3", "4", "4", "5", "5",
                        "6", "6", "7", "7", "8", "8", "9", "9", "+2", "+2"]

        # Listan alkioina mahdolliset villien korttien
        # tyypit, eli värinvaihto- ja +4/värinvaihto-kortti.
        wild_card_types = ["changecolor", "+4_changecolor"]

        # Luodaan ensin tavalliset värikortit väri kerrallaan.
        for colour in colours:

            for number in card_numbers:

                # On mahdollista, että kahdella kortilla on sama
                # nimi, joten tallennetaan kopio listaan eri nimellä.
                if f"{colour}_{number}" in self.__deck_list:
                    self.__deck_list.append(f"{colour}_{number}_2")

                else:
                    self.__deck_list.append(f"{colour}_{number}")

        # Luodaan vielä villit kortit tyyppi kerrallaan.
        for card_type in wild_card_types:

            # Kumpaakin villiä korttia on neljä
            # kappaletta, tehdään neljä toistoa.
            for counter in range(1, 5):

                # Useammalla kortilla on sama nimi, tallennetaan
                # kopiot listaan eri nimellä laskuria hyödyntäen.
                if f"wild_{card_type}" in self.__deck_list:
                    self.__deck_list.append(f"wild_{card_type}_{counter}")

                else:
                    self.__deck_list.append(f"wild_{card_type}")

        # Lopuksi sekoitetaan listassa oleva korttinimet hyödyntämällä
        # random -kirjaston shuffle-metodia, eli toisinsanoen pakka
        # sekoitetaan. Jokaisella pelikerralla kortit ovat eri järjestyksessä.
        shuffle(self.__deck_list)

    def participant_namelist(self):
        """
        Metodi tarkastaa päävalikon "Start game" -nappia
        painettaessa syötekentän arvon kelvollisuuden. Arvon
        ollessa kelvollinen, luodaan syötekentän arvon
        perusteella attribuuttiin tallennettava osallistujalista.
        """

        # Pelaajien mahdolliset nimet listan alkioina.
        player_names = ["Player 1", "Player 2", "Player 3", "Player 4",
                        "Player 5", "Player 6", "Player 7", "Player 8",
                        "Player 9", "Player 10"]

        # Lista, jonne peliin osallistuvien pelaajien nimet tallennetaan.
        participants_list = []

        # Yritetään käsitellä päävalikon syötekentän arvoa.
        try:

            # Haetaan syötekentän arvo peliin
            # osallistuvien pelaajien lukumääräksi.
            players_value = int(self.__player_value.get())

            # Jos haettu arvo ei ole pyydetyllä
            # välillä, päädytään poikkeukseen.
            if players_value < 2 or players_value > 10:
                raise TypeError

            # Lisätään osallistujien nimiä listaan haetun arvon verran.
            for player_number in range(0, players_value):
                participants_list.append(f"{player_names[player_number]}")

            # Tallennetaan osallistujalista attribuuttiin.
            self.__participant_names = participants_list

            # Haettu arvo oli kelvollinen, aloitetaan peliruudun valmistelu.
            self.start_game()

        # Jos päädytään poikkeukseen, toimitaan määrätyllä tavalla.

        # Jos syötekenttään on laitettu jotain muuta kuin numeroita.
        except ValueError:

            # Jaetaan tekstikenttän infoteksti kahdelle riville.
            self.__info_label["text"] = \
                "Error: The number of\nplayers must be numbers."

            # Alustetaan syötekentän sisältö.
            self.reset_fields()

        # Jos syötekenttään laitetut numerot eivät ole pyydetyllä välillä.
        except TypeError:

            # Jaetaan tekstikenttän infoteksti kahdelle riville.
            self.__info_label["text"] = \
                "Error: The number of players\nmust be between 2 and 10."

            # Alustetaan syötekentän sisältö.
            self.reset_fields()

    def start_game(self):
        """
        Metodi valmistelee peliruudun pelin aloittamista varten
        piilottamalla päävalikon osat ja kutsumalla peliruudun
        rakentamiseen vaadittavia metodeja. Metodiin päädytään
        "Start game" -napin painamisen jälkeen, kun ensin on
        tarkastettu päävalikon syötekentän arvojen kelvollisuus.
        """

        # Piilotetaan päävalikon osat näytöltä.
        self.__title_label.grid_forget()
        self.__player_label.grid_forget()
        self.__player_value.grid_forget()
        self.__info_label.grid_forget()
        self.__start_button.grid_forget()
        self.__quit_button.grid_forget()

        # Kutsutaan pelipöydällä sijaitsevat frame-komponentit luovaa metodia.
        # Luodut komponentit tallennetaan aiemmin luotuun attribuuttiin.
        self.get_frames()

        # Seuraavaksi luodaan osallistujaoliot ja jaetaan näille oliolle
        # pakassa olevat kortit. Funktio palauttaa luodut pelaaja- ja
        # pakkaoliot listoina, jotka tallennetaan attribuutteihin.
        self.__deck_objects, self.__participant_objects = \
            deal_cards(self.__deck_list, self.__participant_names)

        # Rakennetaan edellä luodut frame-komponentit,
        # eli luodaan pelaajille jaettuja kortteja vastaavat
        # korttinapit ja asetetaan ne frame-komponenttiin.
        self.build_frames()

        # Sijoitetaan ruudulle vuorossa
        # olevan pelaajan ilmoittava tekstikenttä.
        self.__player_in_turn_label.grid(row=6, column=0, columnspan=3,
                                         sticky=EW)

        # Sijoitetaan näytölle valitun värin kertova tekstikenttä.
        self.__chosen_color_label.grid(row=7, column=0, columnspan=3,
                                       sticky=EW)

        # Kutsutaan pelivuoron toimintoja käsittelevää
        # metodia alkuvalmisteluiden suorittamiseksi.
        self.mr_referee()

    def get_frames(self):
        """"
        Metodi luo peliruudun rakentamisessa vaaditut frame-komponentit
        nosto- ja lyöntipakalle sekä peliin osallistuville pelaajille.
        Luodut kehykset tallennetaan attribuuttiin myöhempää käyttöä varten.
        """

        # Tallennetaan nosto- ja lyöntipakkakehykset sanakirjaan, avaimena
        # frame-komponentin nimi ja hyötykuormana itse frame-komponentti.
        # Kehyksen reunuksen väri musta ja paksuus 3.
        self.__frame_library = {"Take-Deck": Frame(self.__main_window,
                                                   highlightbackground="black",
                                                   highlightthickness=3),
                                "Place-Deck": Frame(self.__main_window,
                                                    highlightbackground="black",
                                                    highlightthickness=3)}

        # Luodaan nostopakan kehykselle nimilappu, ja
        # sijoitetaan se frame-komponentin vasempaan alakulmaan.
        frame_nametag = Label(self.__frame_library["Take-Deck"],
                              text="Take-Deck")
        frame_nametag.grid(row=2, column=0, sticky=SW)

        # Tehdään sama toimenpide lyöntipakalle.
        frame_nametag = Label(self.__frame_library["Place-Deck"],
                              text="Place-Deck")
        frame_nametag.grid(row=2, column=0, sticky=SW)

        # Luodaan peliin osallistuville pelaajille myös kehykset
        # ja kehyksten nimilaput. Sijoitetaan nimilappu oikeaan alakulmaan.
        for name in self.__participant_names:

            # Kehyksen reunuksen väri musta ja paksuus 3.
            self.__frame_library[name] = Frame(self.__main_window,
                                               highlightbackground="black",
                                               highlightthickness=1)

            frame_nametag = Label(self.__frame_library[name], text=f"{name}")

            frame_nametag.grid(row=2, column=0, sticky=SW)

    def build_frames(self, first_round=True):
        """
        Metodi rakentaa peliruudulle sijoitettavat frame-komponentit
        kutsumalla korttinapit luovaa metodia kunkin pelaajan kehyksellä
        ja kädessä olevilla korteilla. Metodi myös sijoittaa pelaajalle
        luodut kortit pelaajan omaan kehykseen.

        :param first_round: bool, onko kyseessä ensimmäinen kierros vai ei.
        """

        # Rakennetaan peliruudun kehykset kehys kerrallaan.
        for frame in self.__frame_library:

            # Käydään osallistujat yksitellen läpi,
            # jotta löydetään kehystä vastaava osallistuja.
            for participant in self.__participant_objects:

                # Jatketaan toimenpiteitä, jos
                # osallistujan nimi vastaa kehyksen nimeä.
                if frame == participant.get_name():

                    # Haetaan osallistujan kädessä olevien korttien nimet.
                    hand = participant.get_deck()

                    # Jos korttinapit luodaan ensimmäistä kertaa.
                    if first_round:

                        # Luodaan pelaajan korteista korttinapit.
                        self.create_card_buttons(self.__frame_library[frame],
                                                 hand)

                    # Jos korttinapit on luotu jo aiemmin.
                    else:

                        # Vaihdetaan joidenkin korttinappien isäntää.
                        self.create_card_buttons(self.__frame_library[frame],
                                                 hand, first_round=False)

                    # Sarakelaskuri korttinappien
                    # kehykseen sijoittamista varten.
                    column = 0

                    # Sijoitetaan pelaajalle luodut korttinapit
                    # yksitellen pelaajan käteen viereisiin sarakkeisiin.
                    for card_1 in hand:

                        # Sijoitetaan nappi sarakelaskurin
                        # mukaiseen sarakkeeseen.
                        self.__deck_library[card_1].grid(row=1, column=column,
                                                         padx=2, pady=2)

                        # Kasvatetaan sarakelaskurin arvoa yhdellä.
                        column += 1

            # Käydään myös Participant -luokan pakkaoliot yksitellen läpi.
            for deck in self.__deck_objects:

                # Jatketaan toimenpiteitä, jos
                # pakan nimi vastaa kehyksen nimeä.
                if frame == deck.get_name():

                    # Haetaan pakassa mahdollisesti olevat kortit.
                    hand = deck.get_deck()

                    # Jos korttinapit luodaan ensimmäistä kertaa.
                    if first_round:
                        self.create_card_buttons(self.__frame_library[frame],
                                                 hand)

                    # Jos korttinapit on luotu jo aiemmin.
                    else:
                        self.create_card_buttons(self.__frame_library[frame],
                                                 hand, first_round=False)

                    # Hypätään pakan yli, jos pakassa ei ole kortteja
                    # lainkaan, esimerkiksi lyöntipakassa pelin alkaessa.
                    if len(hand) == 0:
                        pass

                    # Muussa tapauksessa sijoitetaan kehykseen
                    # vain pakassa päällimmäisenä oleva kortti.
                    else:

                        # Päällimmäisenä oleva kortti on listan lopussa.
                        card = hand[-1]

                        # Sijoitetaan korttinappi kehyksen
                        # ensimmäiseen sarakkeeseen.
                        self.__deck_library[card].grid(row=1, column=0, padx=2,
                                                       pady=2)

        # Kun kehykset on rakennettu, siirrytään
        # kehysten sijoittamiseen peliruudulle.
        self.place_frames()

    def create_card_buttons(self, master, list_of_cardnames, first_round=True):
        """
        Metodi luo ja tallentaa korttinapit sanakirjaan.
        Korttinapeilleasetetaan niiden nimen mukainen kuva,
        ja NORMAL-tila. Lisäksi nappeihin tallennetaan niiden
        isännän ja niiden itsensä nimi. Metodi mahdollistaa
        myös korttinapin isännän vaihtamisen.

        :param master: frame-käyttöliittymäkomponentti,
            kortin isäntä.

        :param list_of_cardnames: list, pakka- tai
            osallistujaolion kädessä olevat kortit
            merkkijonoina (str) listan alkioina.

        :param first_round: bool, onko kyseessä ensimmäinen
            pelikierros vai ei.
        """

        # Jos kyseessä on ensimmäinen pelikierros.
        if first_round:

            # Luodaan laskurit korttien kopioiden nimeämistä varten.

            # Laskuri värinvaihtokorteille.
            counter_cc = 1

            # Laskuri +4/värinvaihtokorteille.
            counter_4cc = 1

            # Käydään isännän kädessä olevat kortit läpi yksitellen.
            for card in list_of_cardnames:

                # Hajotetaan kortin nimi osiin sen
                # kuvatiedoston määritystä varten.
                card_name_parts = card.split("_")

                # Kortin väri ja tyyppi.
                colour_and_type = card_name_parts[0:2]

                # Muodostetaan kortin tiedostonimi ja
                # tallennetaan tiedosto muuttujaan.
                filename = "_".join(colour_and_type)
                card_image_front = PhotoImage(file=f"{filename}_front.png")

                # Jos kortin nimeä ei ole korttinappisanakirjassa.
                if card not in self.__deck_library:

                    # Luodaan korttinappi sanakirjaan,
                    # asetetaan kuva ja NORMAL-tila.
                    self.__deck_library[card] = Button(master,
                                                       image=card_image_front,
                                                       state=NORMAL)

                    # Liitetään korttinappiin napinpainalluksen
                    # jälkeen suoritettava metodi.
                    self.__deck_library[card].bind("<ButtonRelease>",
                                                   self.set_identifiers)

                    # Tallennetaan korttinappiin sen isännän nimi.
                    self.__deck_library[card].identifier_master = master

                    # Tallennetaan korttinappiin myös sen oma nimi.
                    self.__deck_library[card].identifier_card = f"{card}"

                    # Lisätään korttinapin kuvaan toinen viite,
                    # muuten kuva ei tallennu lainkaan.
                    self.__deck_library[card].image = card_image_front

                # Jos kyseessä on värinvaihtokortti.
                elif card == "wild_changecolor":

                    # Luodaan korttinappi sanakirjaan, hyödynnetään
                    # nimeämisessä aiemmin luotua laskuria.
                    # Asetetaan kortin kuva ja NORMAL-tila.
                    self.__deck_library[f"{card}_{counter_cc}"] = Button(master,
                                                                         image=card_image_front,
                                                                         state=NORMAL)

                    # Liitetään korttinappiin napinpainalluksen
                    # jälkeen suoritettava metodi.
                    self.__deck_library[f"{card}_{counter_cc}"].bind("<ButtonRelease>",
                                                                     self.set_identifiers)

                    # Tallennetaan korttinappiin sen isännän nimi.
                    self.__deck_library[f"{card}_{counter_cc}"].identifier_master = master

                    # Tallennetaan korttinappiin myös sen oma nimi.
                    self.__deck_library[f"{card}_{counter_cc}"].identifier_card = f"{card}_{counter_cc}"

                    # Lisätään korttinapin kuvaan toinen viite,
                    # muuten kuva ei tallennu lainkaan.
                    self.__deck_library[f"{card}_{counter_cc}"].image = \
                        card_image_front

                    # Kasvatetaan laskurin arvoa yhdellä.
                    counter_cc += 1

                # Jos kyseessä on +4/värinvaihtokortti.
                elif card == "wild_+4_changecolor":

                    # Luodaan korttinappi sanakirjaan, hyödynnetään
                    # nimeämisessä toista aiemmin luotua laskuria.
                    # Asetetaan kortin kuva ja NORMAL-tila.
                    self.__deck_library[f"{card}_{counter_4cc}"] = Button(master,
                                                                          image=card_image_front,
                                                                          state=NORMAL)

                    # Liitetään korttinappiin napinpainalluksen
                    # jälkeen suoritettava metodi.
                    self.__deck_library[f"{card}_{counter_4cc}"].bind("<ButtonRelease>",
                                                                      self.set_identifiers)

                    # Tallennetaan korttinappiin sen isännän nimi.
                    self.__deck_library[f"{card}_{counter_4cc}"].identifier_master = master

                    # Tallennetaan korttinappiin myös sen oma nimi.
                    self.__deck_library[f"{card}_{counter_4cc}"].identifier_card = f"{card}_{counter_cc}"

                    # Lisätään korttinapin kuvaan toinen viite,
                    # muuten kuva ei tallennu lainkaan.
                    self.__deck_library[f"{card}_{counter_4cc}"].image = \
                        card_image_front

                    # Kasvatetaan toisen laskurin arvoa yhdellä.
                    counter_4cc += 1

                # Jos kyseessä ei ole villi kortti, vaan tavallinen
                # värikortti, jonka aiempi kopio löytyy jo sanakirjasta.
                else:

                    # Luodaan korttinappi sanakirjaan, nimetään korttinappi
                    # aiemmasta korttinapista eroavalla tavalla. Asetetaan
                    # kortin kuva ja NORMAL-tila.
                    self.__deck_library[f"{card}_2"] = Button(master,
                                                              image=card_image_front,
                                                              state=NORMAL)

                    # Liitetään korttinappiin napinpainalluksen
                    # jälkeen suoritettava metodi.
                    self.__deck_library[f"{card}_2"].bind("<ButtonRelease>",
                                                          self.set_identifiers)

                    # Tallennetaan korttinappiin sen isännän nimi.
                    self.__deck_library[f"{card}_2"].identifier_master = master

                    # Tallennetaan korttinappiin myös sen oma nimi.
                    self.__deck_library[f"{card}_2"].identifier_card = f"{card}_2"

                    # Lisätään korttinapin kuvaan toinen viite,
                    # muuten kuva ei tallennu lainkaan.
                    self.__deck_library[f"{card}_2"].image = card_image_front

        # Jos metodia kutsuttaessa on haluttu muokata korttinapin isäntää.
        else:

            # Käydään isännän kädessä olevat kortit läpi yksitellen.
            for card in list_of_cardnames:

                # Hajotetaan kortin nimi osiin sen
                # kuvatiedoston määritystä varten.
                card_name_parts = card.split("_")

                # Kortin väri ja tyyppi.
                colour_and_type = card_name_parts[0:2]

                # Muodostetaan kortin tiedostonimi ja
                # tallennetaan tiedosto muuttujaan.
                filename = "_".join(colour_and_type)
                card_image_front = PhotoImage(file=f"{filename}_front.png")

                # Unohdetaan aiempi korttinappi peliruudulta,
                # ja poistetaan aiemmin luotu samanniminen
                # korttinappi kokonaan sanakirjasta.
                self.__deck_library[card].grid_forget()
                del self.__deck_library[card]

                # Tallennetaan uusi samanniminen korttinappi sanakirjaan.
                # Asetetaan kortin kuva ja NORMAL-tila.
                self.__deck_library[card] = Button(master,
                                                   image=card_image_front,
                                                   state=NORMAL)

                # Liitetään korttinappiin napinpainalluksen
                # jälkeen suoritettava metodi.
                self.__deck_library[card].bind("<ButtonRelease>",
                                               self.set_identifiers)

                # Tallennetaan korttinappiin sen isännän nimi.
                self.__deck_library[card].identifier_master = master

                # Tallennetaan korttinappiin myös sen oma nimi.
                self.__deck_library[card].identifier_card = f"{card}"

                # Lisätään korttinapin kuvaan toinen viite,
                # muuten kuva ei tallennu lainkaan.
                self.__deck_library[card].image = card_image_front

    def place_frames(self):
        """
        Metodi sijoittaa kehyssanakirjassa olevat kehykset ruudulle
        haettujen rivien ja sarakkeiden lukumäärän mukaan. Tämän
        lisäksi metodi sijoittee peliruudulle värinvalintakehyksen.
        """

        # Haetaan peliruudulle rakennettavien
        # rivien ja sarakkeiden lukumäärä.
        rows, columns = self.get_game_screen_size()

        # Lista, sisältää peliruudulle jo sijoitetut kehykset.
        placed_frames = []

        # Laskuri, rivi jolle kehys sijoitetaan peliruudulla.
        row = 1

        # Toistetaan sijoitustoimenpidettä
        # rakennettavien rivien lukumäärän verran.
        for rep in range(0, rows):

            # Laskuri, määrittää riville sijoitettavien kehysten lukumäärän.
            counter = 1

            # Laskuri, sarakkeen johon kehys sijoitetaan.
            column = 0

            # Käydään kehyssanakirja läpi kehys kerrallaan.
            for frame in self.__frame_library:

                # Ohitetaan kehys jos kyseessä nosto- tai lyöntipakan kehys.
                if frame == "Take-Deck" or frame == "Place-Deck":
                    pass

                # Ohitetaan kehys jos se on jo sijoitettu peliruudulle.
                elif frame in placed_frames:
                    pass

                # Jos riville on sijoitettu kolme
                # saraketta, vaihdetaan seuraavalle riville.
                elif counter == 3:
                    row += 1
                    break

                # Muussa tapauksessa sijoitetaan kehys peliruudulle.
                else:

                    # Laskurit määrittävät kehyksen sijainnin.
                    # Venytetään sarake vaakasuunnassa.
                    self.__frame_library[frame].grid(row=row, column=column,
                                                     sticky=EW)

                    # Lisätään riville sijoitettujen kehysten lukumäärää.
                    counter += 1

                    # Hypätään sarakelaskurilla keskimmäinen
                    # sarake yli, sillä siinä sijaitsevat
                    # pakkakehykset sekä värinvalintakehys.
                    column += 2

                    # Lisätään sijoitettu kehys sijoitettujen kehysten listaan.
                    placed_frames.append(frame)

        # Sijoitetaan nostopakka keskimmäiseen sarakkeeseen
        # ja tallennetaan se sijoitettujen kehysten listaan.
        self.__frame_library["Take-Deck"].grid(row=1, column=1, sticky=NS)
        placed_frames.append("Take-Deck")

        # Samat toimenpiteet lyöntipakalle.
        self.__frame_library["Place-Deck"].grid(row=3, column=1, sticky=NS)
        placed_frames.append("Place-Deck")

        # Sijoitetaan vielä lopuksi värinvalintakehys pakkakehysten väliin.
        self.__change_color_frame.grid(row=2, column=1, sticky=NS)

    def get_game_screen_size(self):
        """
        Metodi laskee peliruudulle rakennettavien rivien ja
        sarakkeiden lukumäärän osallistuvien pelaajien
        lukumäärän avulla. Nämä arvot palautetaan kutsujalle.

        :return: int, rakennettavien rivien ja sarakkeiden
            lukumäärä kokonaislukuina.
        """

        # Päävalikon syötekenttään asetettu jo kelvolliseksi
        # todettu osallistuvien pelaajien lukumäärä.
        players_value = int(self.__player_value.get())

        # Peliruudun sarakkeiden lukumäärä, keskellä pakkasarake
        # ja ympärillä sarakkeet osallistujien kehyksiä varten.
        number_of_columns = 1 + 2

        # Jokaiselle peliruudun riville tulee kaksi osallistujakehystä,
        # joten rivien lukumäärä saadaan lisäämällä tasan menneiden
        # rivien lukumäärään mahdollisesti jakojäännöksenä jäävä rivi.
        number_of_rows = (players_value // 2) + (players_value % 2)

        # Palautetaan kutsujalle rivien ja sarakkeiden lukumäärä.
        return number_of_rows, number_of_columns

    def set_identifiers(self, event):
        """
        Setter -metodi. Korttinappia painettaessa korttinappiin
        tallennetut isännän ja kortin nimet tallennetaan luokan
        attribuutteihin, jonka jälkeen kutsutaan pelivuoron
        toimintoja käsittelevää metodia.

        :param event: korttinappi, tapahtuman käynnistänyt
            käyttöliittymäkomponentti.
        """

        # Tallennetaan korttinappiin tallennettu
        # napin isännän nimi attribuuttiin.
        self.__frame_id = event.widget.identifier_master

        # Sama toimenpide nappiin tallennetulle kortin nimelle.
        self.__card_id = event.widget.identifier_card

        # Kutsutaan pelivuoron aikana
        # tapahtuvia toimintoja käsittelevää metodia.
        self.mr_referee()

    def set_color_parameter(self, event):
        """
        Setter-metodi. Värinvalintanappia painettaessa nappiin
        tallennettu värinvalintaparametrin arvo tallennetaan
        attribuuttiin.

        :param event: värinvalintanappi, tapahtuman käynnistänyt
            käyttöliittymäkomponentti.
        """

        # Tallennetaan värinvalintakehyksessä olevaan värinvalintanappiin
        # tallennettu värinvalintaparametrin arvo attribuuttiin.
        self.__selected_color_parameter = event.widget.selected_color

    def get_in_turn(self):
        """
        Getter-metodi. Kutsuttaessa palautetaan vuorossa
        olevan pelaajan nimi merkkijonona ja kasvatetaan
        vuorolaskuriattribuutin arvoa yhdellä.

        :return: str, vuorossa olevan pelaajan nimi.
        """

        # Jos laskurin arvo on suurempi tai yhtäsuuri kuin
        # osallistujien lukumäärä, alustetaan laskurin arvo.
        if self.__in_turn_index >= len(self.__participant_names):
            self.__in_turn_index = 0

        # Valitaan laskurin mukainen pelaaja osallistujalistalta.
        name_to_return = self.__participant_names[self.__in_turn_index]

        # Kasvatetaan laskurin arvoa yhdellä.
        self.__in_turn_index += 1

        # Palautetaan vuorossa olevan pelaajan nimi.
        return name_to_return

    def get_next_in_turn(self):
        """
        Getter-metodi. Kutsuttaessa palautetaan seuraavana
        vuorossa olevan pelaajan nimi merkkijonona.

        :return: str, seuraavana vuorossa olevan pelaajan nimi.
        """

        # Jos laskurin arvo on yhtäsuuri kuin osallistuvien pelaajien
        # lukumäärä, seuraavana vuorossa on listan ensimmäinen pelaaja.
        if self.__in_turn_index == len(self.__participant_names):
            return self.__participant_names[0]

        # Koska aiemmin on korotettu vuorolaskuriattribuutin arvoa
        # yhdellä, seuraavana vuorossa on laskurin mukainen pelaaja listalta.
        else:
            return self.__participant_names[self.__in_turn_index]

    def get_participant(self, requested_participant):
        """
        Getter-metodi. Palauttaa parametrinä saatua pelaajan
        nimeä vastaavan Participant -luokan pelaajaolion.

        :param requested_participant: str, pyydetyn
            osallistujan nimi.

        :return: obj, pyydettyä osallistujan nimeä vastaava
            Participant-luokan pelaajaolio.
        """

        # Käydään pelaajaoliot läpi yksitellen.
        for participant in self.__participant_objects:

            # Jos parametrinä saatu nimi vastaa olion nimeä,
            # palautetaan kyseinen pelaajaolio.
            if requested_participant == participant.get_name():
                return participant

    def make_card_decisions(self, in_turn, participant_in_turn,
                            next_in_turn_object):
        """
        Metodi, joka reagoi painettuun korttinappiin tallennettuun
        nimeen ja tekee toimenpiteitä riippuen tallennetuista
        arvoista. Metodia kutsutaan jokaisen pelivuoron aikana
        "mr_referee"-metodista.

        :param in_turn: str, vuorossa olevan pelaajan nimi.

        :param participant_in_turn: obj, vuorossa olevaa
            pelaajaa vastaava Participant-luokan olio.

        :param next_in_turn_object: obj, seuraavana vuorossa
            olevaa pelaajaa vastaava Participant-luokan olio.
        """

        # Jaetaan korttinappiin tallennettu kortin nimi osiin.
        card_id_parts = self.__card_id.split("_")

        # Jos painetun korttinapin isäntä on nostopakka,
        # pelaaja haluaa nostaa kortin pakasta.
        if self.__frame_id == self.__frame_library["Take-Deck"]:

            # Kutsutaan kortin siirron suorittavaa funktiota.
            transfer_card(self.__card_id, "Take-Deck", participant_in_turn,
                          self.__deck_objects)

            # Päivitetään korttinapit peliruudulle.
            self.build_frames(first_round=False)

            # Haetaan seuraavana vuorossa olevan
            # pelaajan pöytään lyötäväksi sopivat kortit.
            self.__suitable_cards = self.suitable_cards(next_in_turn_object)

        # Jos painettu korttinappi on +2 -kortti.
        elif card_id_parts[1] == "+2":

            # Lyödään +2 -kortti pöytään, eli siirretään se lyöntipakkaan.
            transfer_card(self.__card_id, "Place-Deck",
                          participant_in_turn, self.__deck_objects)

            # Seuraava pelaaja nostaa kaksi korttia pakasta.
            for repetition in range(0, 2):
                transfer_card(self.__card_id, "Take-Deck",
                              next_in_turn_object, self.__deck_objects)

            # Päivitetään korttinapit peliruudulle.
            self.build_frames(first_round=False)

            # Haetaan seuraavana vuorossa olevan
            # pelaajan pöytään lyötäväksi sopivat kortit.
            self.__suitable_cards = self.suitable_cards(next_in_turn_object)

        # Jos painettu korttinappi on +4/värinvaihto -kortti.
        elif card_id_parts[1] == "+4":

            # Lyödään +4/värinvaihto -kortti pöytään,
            # eli siirretään se lyöntipakkaan.
            transfer_card(self.__card_id, "Place-Deck",
                          participant_in_turn, self.__deck_objects)

            # Seuraava pelaaja nostaa neljä korttia pakasta.
            for repetition in range(0, 4):
                transfer_card(self.__card_id, "Take-Deck",
                              next_in_turn_object, self.__deck_objects)

            # Vaihdetaan seuraavaksi lyötävän kortin
            # sallittua väriä, jos pelaaja unohti
            # valita värin ennen kortin lyömistä pöytään.
            if self.__selected_color_parameter == "":

                # Mahdolliset kortin värit listan alkioina.
                card_colors = ["red", "blue", "yellow", "green"]

                # Satunnainen indeksi random -kirjaston randint-metodin avulla.
                random_color = randint(0, 3)

                # Valitaan kortin väri satunnaisella indeksillä.
                self.__selected_color_parameter = card_colors[random_color]

                # Muokataan valitun värin ilmoittavaa tekstikenttää.
                self.__chosen_color_label.configure(text=f"{in_turn} "
                                                         f"forgot to "
                                                         f"choose the "
                                                         f"color. The "
                                                         f"color "
                                                         f"{self.__selected_color_parameter}"
                                                         f" was chosen "
                                                         f"randomly.")

            # Tekstikentän teksti, jos pelaaja muisti valita värin itse.
            else:

                self.__chosen_color_label.configure(text=f"{in_turn} has "
                                                         f"chosen the "
                                                         f"color {self.__selected_color_parameter}.")

            # Tekstikentän taustaväri valitun värin mukaiseksi.
            self.__chosen_color_label.configure(
                background=f"{self.__selected_color_parameter}")

            # Päivitetään korttinapit peliruudulle.
            self.build_frames(first_round=False)

            # Haetaan seuraavana vuorossa olevan
            # pelaajan pöytään lyötäväksi sopivat kortit.
            self.__suitable_cards = self.suitable_cards(next_in_turn_object,
                                                        change_color=True)

            # Nollataan valitun värin sisältävä attribuutti.
            self.__selected_color_parameter = ""

        # Jos painettu korttinappi on värinvaihtokortti.
        elif card_id_parts[1] == "changecolor":

            # Lyödään värinvaihtokortti pöytään,
            # eli siirretään se lyöntipakkaan.
            transfer_card(self.__card_id, "Place-Deck",
                          participant_in_turn, self.__deck_objects)

            # Vaihdetaan seuraavaksi lyötävän kortin
            # sallittua väriä, jos pelaaja unohti
            # valita värin ennen kortin lyömistä pöytään.
            if self.__selected_color_parameter == "":

                # Mahdolliset kortin värit listan alkioina.
                card_colors = ["red", "blue", "yellow", "green"]

                # Satunnainen indeksi random -kirjaston randint-metodin avulla.
                random_color = randint(0, 3)

                # Valitaan kortin väri satunnaisella indeksillä.
                self.__selected_color_parameter = card_colors[random_color]

                # Muokataan valitun värin ilmoittavaa tekstikenttää.
                self.__chosen_color_label.configure(text=f"{in_turn} "
                                                         f"forgot to "
                                                         f"choose the "
                                                         f"color. The "
                                                         f"color "
                                                         f"{self.__selected_color_parameter}"
                                                         f" was chosen "
                                                         f"randomly.")

            # Tekstikentän teksti, jos pelaaja muisti valita värin itse.
            else:

                self.__chosen_color_label.configure(text=f"{in_turn} has "
                                                         f"chosen the "
                                                         f"color {self.__selected_color_parameter}.")

            # Tekstikentän taustaväri valitun värin mukaiseksi.
            self.__chosen_color_label.configure(
                background=f"{self.__selected_color_parameter}")

            # Päivitetään korttinapit peliruudulle.
            self.build_frames(first_round=False)

            # Haetaan seuraavana vuorossa olevan
            # pelaajan pöytään lyötäväksi sopivat kortit.
            self.__suitable_cards = self.suitable_cards(next_in_turn_object,
                                                        change_color=True)

            # Nollataan valitun värin sisältävä attribuutti.
            self.__selected_color_parameter = ""

        # Jos painettu korttinappi on pelaajan kädessä,
        # eli pelaaja haluaa lyödä kortin lyöntipakkaan.
        else:

            # Lyödään kortti pöytään, eli siirretään se lyöntipakkaan.
            transfer_card(self.__card_id, "Place-Deck", participant_in_turn,
                          self.__deck_objects)

            # Päivitetään korttinapit peliruudulle.
            self.build_frames(first_round=False)

            # Haetaan seuraavana vuorossa olevan
            # pelaajan pöytään lyötäväksi sopivat kortit.
            self.__suitable_cards = self.suitable_cards(next_in_turn_object)

    def suitable_cards(self, participant, first_round=False,
                       change_color=False):
        """
        Metodi tarkastelee parametrinä saadun pelaajan kortteja
        ja määrittää mitkä niistä sopivat pöytään lyötäviksi,
        ottaen huomioon lyöntipakassa päällimmäisenä olevan kortin.

        :param participant: obj, pelaaja, jonka kädessä olevia
            kortteja tarkastellaan.

        :param first_round: bool, onko kyseessä ensimmäinen
            pelivuoro vai ei.

        :param change_color: bool, onko päällimmäisenä korttina
            villi kortti vai ei.

        :return: list, pöytään lyötäviksi sopivien korttien nimet
            merkkijonoina (str) listan alkioina.
        """

        # Jos kyseessä on ensimmäinen pelivuoro, kaikki pelaajan
        # kädessä olevat kortit sopivat pöytään lyötäväksi.
        if first_round:
            return participant.get_deck()

        # Haetaan lyöntipakassa olevat kortit.
        place_deck = self.__deck_objects[1].get_deck()

        # Jos lyöntipakassa on nolla korttia, kaikki pelaajan
        # kädessä olevat kortit sopivat pöytään lyötäväksi.
        if len(place_deck) == 0:
            return participant.get_deck()

        # Lyöntipakan päällimmäinen kortti.
        placed_top_card = place_deck[-1]

        # Jaetaan päällimäisen kortin nimi osiin jatkotutkimuksia varten.
        top_card_parts = placed_top_card.split("_")

        # Päällimmäisen kortin väri.
        top_color = top_card_parts[0]

        # Saman kortin tyyppi.
        top_type = top_card_parts[1]

        # Lista, jonne tallennetaan lyötäväksi sopivien korttien nimet.
        suitable_cardnames = []

        # Jos pöytään on lyöty villi kortti.
        if change_color:

            # Käydään vuorossa olevan kortit läpi yksitellen.
            for card in participant.get_deck():

                # Jaetaan kortin nimi osiin.
                card_name_parts = card.split("_")

                # Kortin väri.
                card_color = card_name_parts[0]

                # Jos attribuutin sisältämä väri vastaa
                # kortin väriä, sopii kortti pöytään lyötäväksi.
                if self.__selected_color_parameter == card_color:
                    suitable_cardnames.append(card)

                # Lisätään aina käyvät kortit eli
                # värinvaihto- ja +4/värinvaihto -kortit.
                elif card_color == "wild":
                    suitable_cardnames.append(card)

        # Jos pöytään lyöty kortti ei ole villi kortti.
        else:

            # Käydään vuorossa olevan kortit läpi yksitellen.
            for card in participant.get_deck():

                # Jaetaan kortin nimi osiin.
                card_name_parts = card.split("_")

                # Kortin väri.
                card_color = card_name_parts[0]

                # Kortin tyyppi.
                card_type = card_name_parts[1]

                # Jos kädessä olevan kortin väri vastaa lyöntipakan
                # päällimmäisen kortin väriä, sopii kortti pöytään lyötäväksi.
                if card_color == top_color:
                    suitable_cardnames.append(card)

                # Jos kädessä olevan kortin tyyppi vastaa
                # lyöntipakan päällimmäisen kortin tyyppiä,
                # sopii kortti pöytään lyötäväksi.
                elif card_type == top_type:
                    suitable_cardnames.append(card)

                # Lisätään aina käyvät kortit eli
                # värinvaihto- ja +4/värinvaihto -kortit.
                elif card_color == "wild":
                    suitable_cardnames.append(card)

        # Palautetaan kutsujalle sopivien korttien nimet sisältävä lista.
        return suitable_cardnames

    def update_cards_state(self, turn_end=False):
        """
        Metodi päivittää kortit DISABLED- tai NORMAL-tilaan riippuen
        alkaako vai päättyykö vuoro. Lisäksi muuttaa nostopakan kortit
        NORMAL-tilaan.

        :param turn_end: bool, alkaako vai päättyykö vuoro.
        """

        # Jos kyseessä ei ole vuoron lopetus.
        if not turn_end:

            # Käydään läpi korttinapit yksitellen.
            for name in self.__deck_library:

                # Jos korttinapin nimi ei ole sopivien korttien listassa.
                if name not in self.__suitable_cards:

                    # Muutetaan korttinappi DISABLED-tilaan.
                    self.__deck_library[name].configure(state=DISABLED)

        # Jos kyseessä vuoron lopetus.
        else:

            # Käydään lyötäväksi sopivat kortit yksitellen läpi.
            for name in self.__suitable_cards:

                # Muutetaan nämä korttinapit NORMAL-tilaan.
                self.__deck_library[name].configure(state=NORMAL)

        # Haetaan nostopakassa olevien korttien nimet.
        take_decks_deck = self.__deck_objects[0].get_deck()

        # Nostopakan kortit yksitellen läpi.
        for card_name in take_decks_deck:

            # Muutetaan nostopakan kortit NORMAL-tilaan.
            self.__deck_library[card_name].configure(state=NORMAL)

    def flip_cards(self, next_in_turn_object):
        """
        Metodi kääntää seuraavana vuorossa olevan pelaajaolion
        kortit kuvapuoli ylöspäin, ja muiden pelaajaolioiden
        kortit selkäpuoli ylöspäin. Lisäksi metodi kääntää myös
        nostopakassa olevat kortit kuvapuoli ylöspäin.

        :param next_in_turn_object: obj, seuraavana vuorossa
            oleva Participant -luokan pelaajaolio.
        """

        # Seuraavana vuorossa olevan pelaajan kortit.
        next_in_turn_deck = next_in_turn_object.get_deck()

        # Seuraavana vuorossa olevan pelaajan kortit kuvapuoli ylöspäin,
        # eli näihin kortteihin muutetaan niiden oma alkuperäinen kuva.
        # Käydään kortit läpi yksitellen.
        for card_name in next_in_turn_deck:

            # Hajotetaan kortin nimi osiin sen
            # kuvatiedoston määritystä varten.
            card_name_parts = card_name.split("_")

            # Kortin väri ja tyyppi.
            colour_and_type = card_name_parts[0:2]

            # Muodostetaan kortin tiedostonimi ja
            # tallennetaan tiedosto muuttujaan.
            filename = "_".join(colour_and_type)
            card_image_front = PhotoImage(file=f"{filename}_front.png")

            # Asetetaan tallennettu kuva korttinappiin ja lisätään korttinapin
            # kuvaan toinen viite, muuten kuva ei tallennu lainkaan.
            self.__deck_library[card_name].configure(image=card_image_front)
            self.__deck_library[card_name].image = card_image_front

        # Käännetään kaikkien muiden pelaajien kortit selkäpuoli ylöspäin.
        # Käydään pelaajaoliot läpi yksitellen.
        for participant in self.__participant_objects:

            # Ohitetaan olio, jos kyseessä seuraavana vuorossa oleva pelaaja.
            if participant is next_in_turn_object:
                pass

            # Muiden pelaajien kortit selkäpuoli ylöspäin.
            else:

                # Haetaan pelaajaolion pakka.
                participant_deck = participant.get_deck()

                # Käydään pelaajaolion pakassa olevat kortit yksitellen läpi.
                for card_name in participant_deck:

                    # Asetetaan attribuuttiin tallennettu kortin
                    # selkäpuolen kuva korttinappiin ja lisätään korttinapin
                    # kuvaan toinen viite, muuten kuva ei tallennu lainkaan.
                    self.__deck_library[card_name].configure(image=self.__card_image_back)
                    self.__deck_library[card_name].image = self.__card_image_back

        # Haetaan nostopakassa olevat kortit.
        take_decks_deck = self.__deck_objects[0].get_deck()

        # Käännetään myös nostopakan kortit selkäpuoli ylöspäin.
        for card_name in take_decks_deck:

            # Suoritetaan sama toimenpide, joka
            # tehtiin muiden pelaajien korteille.
            self.__deck_library[card_name].configure(image=self.__card_image_back)
            self.__deck_library[card_name].image = self.__card_image_back

    def show_win_screen(self):
        """
        Metodia kutsutaan kun joku pelaajista on voittanut
        pelin. Metodi unohtaa peliruudulla käytetyt kehykset
        ja muut käyttöliittymäkomponentit sekä sijoittelee
        voittoruudulla tarvittavat komponentit. Lopuksi
        metodi kutsuu animaatiota päivittävää metodia.
        """

        # Unohdetaan peliruudulta jokainen kortteja sisältävä
        # kehys, eli pakkakehykset ja pelaajien kehykset.
        for frame in self.__frame_library:
            self.__frame_library[frame].grid_forget()

        # Unohdetaan värinvaihtokehys.
        self.__change_color_frame.grid_forget()

        # Lopuksi unohdetaan myös vuorossa olevan
        # pelaajan ja valitun värin ilmoittavat tekstikentät.
        self.__player_in_turn_label.grid_forget()
        self.__chosen_color_label.grid_forget()

        # Muokataan aiemmin unohdettua päävalikon lopetusnappia.
        self.__quit_button.configure(background="red", foreground="black")

        # Sijoitetaan lopetusnappi voittoruudulle.
        self.__quit_button.grid(row=3, column=0, rowspan=2, sticky=NW+SE)

        # Sijoitetaan voittoruudulle tekstikenttä,
        # jossa animaation kuvia tullaan päivittämään.
        self.__prize_animation_label.grid(row=1, column=0, sticky=NW+SE)

        # Sijoitetaan loppuruudulle myös onnittelutekstikenttä.
        self.__congratulations_label.grid(row=2, column=0, sticky=NW+SE)

        # Muokataan onnittelutekstikenttä onnittelemaan voittanutta pelaajaa.
        self.__congratulations_label.configure(text=f"Congratulations "
                                                    f"{self.__winner}, you are"
                                                    f" the new UUNO_master!")

        # Siirrytään animaatiota päivittävään metodiin.
        self.refresh_animation()

    def refresh_animation(self):
        """
        Metodi hallinnoi animaatiossa vaadittavien kuvien
        päivittämistä. Animaatiossa olevaa kuvaa vaihdetaan
        200 millisekunnin välein kolmen kuvatiedoston kesken.
        """

        # Animaation kuvan valitsemiseen käytettävä indeksi.
        counter = self.__refresh_counter

        # Muutetaan animaation kuvaa sille laaditussa tekstikentässä indeksin
        # mukaiseksi. Animaatiossa on kolme kuvaa, jonka vuoksi vuorossa oleva
        # kuvatiedosto määritetään numeron kolme jakojäännöksen avulla.
        self.__prize_animation_label.configure(image=self.__prize_photos[counter % 3])

        # Kasvatetaan indeksin arvoa yhdellä.
        self.__refresh_counter += 1

        # Pääikkuna odottaa 200 millisekuntia, jonka jälkeen toistaa
        # animaatiota päivittävän metodin uudelleen. Animaatiota toistetaan,
        # kunnes pelaaja on tyytyväinen ja sulkee pelin napinpainalluksella.
        self.__main_window.after(200, self.refresh_animation)

    def reset_fields(self):
        """
        Metodi tyhjentää kutsuttaessa päävalikossa olevan
        syötekentässä olevan arvon.
        """

        # Tyhjentää päävalikon syötekentän alusta loppuun.
        self.__player_value.delete(0, END)

    def open_window(self):
        """
        Metodi avaa kutsuttaessa pääikkunaa
        pyörittävän silmukan.
        """

        # Käynnistetään silmukka.
        self.__main_window.mainloop()

    def close_window(self):
        """
        Metodi sulkee kutsuttaessa pääikkunaa pyörittävän
        silmukan. Metodi on kytkettynä päävalikossa ja
        voittoruudussa käytettyyn "Quit game" -nappiin.
        """

        # Tuhotaan pääikkuna totaalisesti.
        self.__main_window.destroy()


class Participant:
    """
    Luokka toteuttaa UUNO! -pelissä hyödynnettävät osallistujaoliot,
    joihin kuuluvat niin pelaajaoliot, kuin nosto- ja lyöntipakkaoliot.
    Luokkaan kuuluvilla olioilla on nimi ja lista, joka sisältää niiden
    kädessä olevien pelikorttien nimet.
    """

    def __init__(self, name, hand=None):
        """
        Rakentaja, alustaa pelaaja- tai pakkaolioille nimen ja listan,
        joka sisältää niiden kädessä olevien pelikorttien nimet.

        :param name: str, pelaajan tai pakan nimi.

        :param hand: list, pelaajan tai pakan kädessä olevien
            pelikorttien nimet. Voidaan antaa parametrinä, kuten
            ostopakkaa alustaessa tehdään, mutta tämä ei ole pakollista.
        """

        # Pelaajalle tai pakalle asetettu nimi.
        self.__name = name

        # Jos pelaajalle tai pakalle ei ole alustettaessa määritetty
        # käden sisältöä, on korttien nimet sisältävä lista tyhjä.
        if hand is None:
            self.__hand = []

        # Muussa tapauksessa kädessä on suoraan
        # parametrinä saatu lista korttien nimistä.
        else:
            self.__hand = hand

    def get_name(self):
        """
        Getter-metodi. Palauttaa kutsuttaessa
        pelaaja- tai pakkaoliolle alustetun nimen.

        :return: str, pelaajalle tai pakalle alustettu nimi.
        """

        return self.__name

    def get_deck(self):
        """
        Getter-metodi. Palauttaa kutsuttaessa pelaaja- tai
        pakkaolion kädessä olevien korttien nimet sisältävän listan.

        :return: list, pelaajalla- tai pakalla kädessä
            olevat korttien nimet listan alkioina.
        """

        return self.__hand

    def shuffle_deck(self):
        """
        Metodi sekoittaa kutsuttaessa pelaaja- tai
        pakkaolion kädessä olevat kortit random -kirjaston
        shuffle-metodin avulla.
        """

        shuffle(self.__hand)

    def give_card(self, card_name):
        """
        Metodi poistaa parametrinä saadun
        kortin pelaajan kädestä.

        :param card_name: str, kädestä poistettavan
            kortin nimi.
        """

        self.__hand.remove(card_name)

    def receive_card(self, card_name):
        """
        Metodi lisää parametrinä saadun
        kortin pelaajan käteen.

        :param card_name: str, käteen lisättävän
            kortin nimi.
        """

        self.__hand.append(card_name)

    def transfer_card(self, card_name, participant_receive):
        """
        Metodi siirtää parametrinä saadun kortin
        toisen olion kädestä toisen olion käteen.

        :param card_name: str, siirrettävän kortin nimi.

        :param participant_receive: obj, Participant -luokan
            olio jolle parametrinä saatu kortti annetaan.
        """

        # Annetaan parametrinä saatu kortti parametrinä saadun olion käteen.
        participant_receive.receive_card(card_name)

        # Poistetaan parametrinä saatu kortti olion kädestä.
        self.give_card(card_name)


def create_participants(participants_names_list):
    """
    Funktio luo parametrinä saamistaan pelaajien nimistä
    Participant -luokan oliot. Nämä oliot tallennetaan
    listan alkioiksi ja palautetaan kutsujalle.

    :param participants_names_list: list, UUNO! -peliin
        osallistuvien pelaajien nimet merkkijonoina (str)
        listan alkioina.

    :return: list, Participant -luokan pelaajaoliot
        listan alkioiksi tallennettuna.
    """

    # Lista, jonne tallennetaan luodut oliot.
    participants_obj_list = []

    # Luodaan parametrinä saaduista pelaajien nimistä
    # nimiä vastaavat Participant -luokan oliot.
    for name in participants_names_list:
        participants_obj_list.append(Participant(name))

    # Palautetaan listaan tallennetut oliot kutsujalle.
    return participants_obj_list


def deal_cards(deck, participants_names_list):
    """
    Funktio luo Participant -luokan nosto- ja lyöntipakkaoliot.
    Nostopakkaolion alustukseen annetaan parametrinä aiemmin
    luotu lista, joka sisältää korttipakan korttien nimet (str)
    listan alkioina. Lisäksi funktio kutsuu pelaajaoliot luovaa
    funktiota, jonka lisäksi jakaa luoduille pelaajaolioille
    kullekin seitsemän pelikorttia.

    :param deck: list, pelikorttien nimet (str) listan alkioina
        eli korttipakka.

    :param participants_names_list: list, peliin osallistuvien
        pelaajien nimet (str) listan alkioina.

    :return: list, palauttaa kaksi listaa, joista toisessa
        luodut pakkaoliot ja toisessa luodut pelaajaoliot.
    """

    # Luodaan nostopakkaolio.
    take_deck = Participant("Take-Deck", deck)

    # Luodaan lyöntipakkaolio.
    place_deck = Participant("Place-Deck")

    # Kutsutaan pelaajaoliot luovaa funktiota.
    participants_obj_list = create_participants(participants_names_list)

    # Lista, jonka alkioina nostopakasta nostettujen korttien nimet.
    take_deck_cards_to_remove = []

    # Jaetaan kullekin pelaajaoliolle 7 korttia.
    for participant in participants_obj_list:

        # Laskuri, pelaajaoliolle jaettujen korttien lukumäärä.
        cards_dealt = 0

        # Jaetaan nostopakan kortteja pelaajalle.
        for card in take_deck.get_deck():

            # Poistutaan silmukasta, jos kortteja jaettu riittävä määrä.
            if cards_dealt == 7:
                break

            # Muuten jaetaan pelaajaoliolle kortti.
            else:

                # Annetaan pelaajaolion käteen kortti.
                participant.receive_card(card)

                # Lisätään annettu kortti nostettujen korttien listaan.
                take_deck_cards_to_remove.append(card)

                # Kortti jaettu, kasvatetaan laskurin arvoa yhdellä.
                cards_dealt += 1

        # Poistetaan listaan lisätyt kortit nostopakasta yksitellen.
        for card_to_remove in take_deck_cards_to_remove:

            take_deck.give_card(card_to_remove)

        # Tyhjennetään poistettavien korttien lista, jonka jälkeen
        # jaetaan seuraavalle pelaajaoliolle pyydetyt 7 korttia.
        take_deck_cards_to_remove = []

    # Tallennetaan pakkaoliot listan alkioiksi.
    decks_list = [take_deck, place_deck]

    # Palautetaan pakka- ja pelaajaoliot listojen alkioina kutsujalle.
    return decks_list, participants_obj_list


def transfer_card(button, deck, participant_object, deck_objects,
                  reset_decks=False):
    """
    Funktio hallinnoi olioiden välisiä korttien siirtoja.
    Siirrot ovat mahdollisia niin pelaajalta pelaajalle, kuin
    myös pelaajalta pakkaan ja pakasta pelaajalle.

    :param button: str, korttinappiin tallennettu kortin nimi.

    :param deck: str, pakkaolion nimi, jonne tai josta
        siirtoa ollaan suorittamassa.

    :param participant_object: obj, pelaajaolio jolle tai
        jolta korttia ollaan siirtämässä.

    :param deck_objects: list, Participant -luokan pakkaoliot
        listan alkioina.

    :param reset_decks: bool, onko kyseessä lyöntipakan tyhjennys
        nostopakkaan vai ei.
    """

    # Jos kyseessä on lyöntipakan tyhjennys nostopakkaan.
    if reset_decks:

        # Nostopakkaolio.
        take_deck = deck_objects[0]

        # Lyöntipakkaolio.
        place_deck = deck_objects[1]

        # Siirretään kortti lyöntipakkaoliolta nostopakkaoliolle.
        place_deck.transfer_card(button, take_deck)

    # Jos parametrinä saatu pakkaolion nimi on nostopakka.
    elif deck == deck_objects[0].get_name():

        # Nostopakkaolio.
        take_deck = deck_objects[0]

        # Nostopakassa olevat kortit.
        take_decks_deck = take_deck.get_deck()

        # Nostettava kortti, eli nostopakan päällimmäinen kortti.
        card_to_draw = take_decks_deck[-1]

        # Siirretään nostopakan päällimmäinen kortti pelaajan käteen.
        take_deck.transfer_card(card_to_draw, participant_object)

    # Jos parametrinä saatu pakkaolion nimi on lyöntipakka.
    else:

        # Lyöntipakkaolio.
        place_deck = deck_objects[1]

        # Lyöjä, eli parametrinä saatu pelaajaolio.
        placer = participant_object

        # Siirretään parametrinä saatu kortti pelaajalta lyöntipakkaan.
        placer.transfer_card(button, place_deck)


def main():

    gui = GraphicalUserInterface()
    gui.open_window()


if __name__ == "__main__":
    main()
