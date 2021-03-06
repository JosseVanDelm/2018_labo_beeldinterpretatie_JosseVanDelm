# Gebruik code
Dit programma classificeert rijpe aardbeien.
* gebruik `--training_image=` met het absolute pad naar de afbeelding om een afbeelding te kiezen om op te trainen.
* gebruik `--inference_image=` met het absolute pad naar de afbeelding om een afbeelding te kiezen om de inferentie op uit te voeren.
* gebruik `--help` als je een beetje in de war bent of als je geen zin hebt om deze documentatie te lezen en het allemaal liever zelf uitzoekt.

Gebruik eender welke toets om door het programma te navigeren.
Om punten aan te duiden kan je de muis gebruiken:
* **linkermuisknop** voegt een punt toe.
* **rechtermuisknop** verwijdert het laatst toegevoegde punt.
* **middelmuisknop** laat een lijst zien van alle geselecteerde punten.

**Tips om te trainen:**
* Duid de aardbeien aan indien er gevraagd word om *foreground* te selecteren.
* Duid al de rest aan indien er gevraagd word om *background* te selecteren.
  Duid ook zeker onrijpe aardbeien aan bij background, dit verbeterd de resultaten.

**Het programma werkt niet als er geen punten gekozen worden om op te trainen**

Voor de volledigheid staat hieronder nog eens de volledige opdracht:

# Sessie 5 - Machine Learning

Een aardbei plukker wil graag mee gaan met de tijd en wil zijn manuele arbeid
reduceren. Hiervoor heeft hij aan ons gevraag om een automatische aarbei-pluk
machine te bouwen, die in staat is rijpe aardbeien te onderscheiden van onrijpe
aardbeien. In dit labo gaan jullie proberen om met behulp van machine learning
rijpe aarbeien te detecteren.

* **Opdracht 1**: Maak een interface waarop je op basis van *mouse callbacks*
  pixels kan aanklikken. Bij links klikken, sla je een extra punt op. Rechts
  klikken verwijder je het laatst toegevoegde punt. En waarbij de middelste knop
  een lijst toont met alle opgeslagen punten.

* **Opdracht 2**: Op basis van de geselecteerd punten bepaalje een descriptor,
  in dit geval de HSV representatie van de pixel.

* **Opdracht 3**: Train met deze data een K-Nearest-Neighbor classifier, een
  Normal Bayes Classifier en een Support Vector Machine

* **Opdracht 4**: Classificeer de pixels van de afbeelding met deze technieken
  en visualiseer met een masker de resterende pixels.

* **EXTRA**: Hoe kan je de segmentatie verbeteren? Denk na over het feit dat een
  rijpe aarbei meer rood en minder groen componenten geven.
