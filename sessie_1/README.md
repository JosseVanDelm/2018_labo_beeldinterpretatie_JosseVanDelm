# Gebruik code

voor een overzicht van alle commando's kan hulp gevraagd worden via de `--help`-vlag.
Overzicht van andere vlaggen:
* `--image` zet het pad naar de afbeelding die door de pipeline gestuurd moet worden.
* `--mode` kan een waarde aannemen van:
    * `0` voor "skin segmentation" (opdracht 1.1 deel 1).
    * `1` voor "text segmentation" (opdracht 1.1 deel 2).
    * `2` voor een geavanceerdere versie van "skin segmentation" die convexHull approximation gebruikt. (opdracht 1.2)

**Je kan voor het afbeeldingspad best een absoluut pad gebruiken in Code::blocks**.

Hieronder staat voor de volledigheid nog eens de opdracht.
Om door de verschillende delen van de pipeline te gaan druk je telkens op **een willekeurige toets** op het toetsenbord.

# Sessie 1 - pixel manipulatie

## Opdracht 1.1: Thresholding

* Lees afbeelding `imageColor.png` en segmenteer de skin pixels
   * Gebruik hiervoor een filter uit de literatuur in het RGB domein: `(RED>95) && (GREEN>40) && (BLUE>20) && ((max(RED,max(GREEN,BLUE)) - min(RED,min(GREEN,BLUE)))>15) && (abs(RED-GREEN)>15) && (RED>GREEN) && (RED>BLUE);`
   * Visualiseer naast het masker (binaire pixels die tot huid behoren) dat je bekomt ook de resulterende pixelwaarden.
* Lees afbeelding `imageBimodal.png` in en segmenteer de tekst
   * Start met OTSU thresholding - wat gaat er mis?
   * Hoe kunnen we het originele beeld verbeteren om dit tegen te gaan?
      * Histogram equalization
      * CLAHE: Contrast Limited Adaptive Histogram Equalization
   * Merk je bepaalde problemen bij beide technieken?

## Opdracht 1.2: Erosie en dilatie

* Haal afbeelding `imageColorAdapted` door je thresholding pipeline en ga na wat er mis gaat.
* Gebruik `opening`, `closing`, `dilatie`, `erosie` om het binaire masker op te schonen en ruis te onderdrukken.
* Kan je gescheiden ledematen terug met elkaar verbinden?
* Kun je niet volledige binaire regio's terug vullen?
* Probeer in resulterende blobs regios weg te werken

Bekijk zeker `findContours()`, `convexHull` en `drawContours`!
