# Gebruik Code

Voor een overzicht van alle mogelijke opties kan hulp gevraagd worden via de `--help` vlag. Overzicht van de andere vlaggen:
* `--image1` zet het pad naar afbeelding 1
* `--image2` zet het pad naar afbeelding 2
* `--image3` zet het pad naar afbeelding 3
* `--image4` zet het pad naar afbeelding 4

**Al deze opties moeten ingesteld worden, anders werkt het programma niet.**

**Je kan voor het afbeeldingspad best een absoluut pad gebruiken in Code::blocks**.

Hieronder staat voor de volledigheid nog eens de opdracht.
Om door de verschillende delen van de pipeline te gaan druk je telkens op **een willekeurige toets** op het toetsenbord.
Voor deel 1 en 2 van de opdracht worden alle afbeeldingen apart getoond.
Bij deel 3 en 4 kan men de gewenste afbeelding selecteren met de sliders.

Bij deel 3 kan er enkel via sliders gekozen worden voor een minimum en maximum waarde van H,
en voor een S en V waarde waarboven alle waarden worden toegelaten.
Ik vond immers dat er reeds goede resultaten bekomen konden worden door de parameters van volgende vergelijking te gebruiken
```
mask_hsv = ((image_Hue<H_max) | (image_Hue>H_min)) & (image_Saturation>S) & (image_Value>V)
```
**Het heeft bijgevolg geen zin om een H-min waarde te selecteren die kleiner is dan de H-max waarde.**

# Sessie 2 - colour space manipulation

Omdat autonome wagens heel erg in de opmars zijn willen we dat deze wagens zich ook aan de verkeersregels houden zoals de maximaal toegelaten snelheid. Hiervoor gaan we een camera in de auto monteren en een computer die verkeersborden zal detecteren en analyseren. Jouw opdracht is een stukje software schrijven om de rode verkeersborden te filteren van de achtergrond zodat deze later kunnen geanalyseerd worden.

* Opdracht 1: Segmenteer de verkeersborden in de BGR kleurenruimte
   - Wat zijn de voordelen?
   - Wat zijn de nadelen?
* Opdracht 2: Segmenteer de verkeersborden in de HSV kleurenruimte
   - Wat zijn de voordelen?
   - Wat zijn de nadelen?
* Opdracht 3: Gebruik connected component analyse om enkel het stopbord over te houden
* Opdracht 4: Gebruik een trackbar om de optimale threshold waardes te bepalen
