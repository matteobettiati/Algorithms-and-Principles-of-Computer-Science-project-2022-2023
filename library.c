#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ISTRUZIONE 20

int numero_stazioni = 0;

// max heap per la stazione
typedef struct {
    int distanza;
    int numero_macchine;
    int limite_macchine;
    int *macchine;
} Stazione;

// guarda la macchina con massima autonomia
int massima_autonomia(Stazione *stazione) {
    if (stazione->numero_macchine == 0)
        return -1;
    return stazione->macchine[0];
}

// crea stazione
Stazione *crea_stazione(int distanza, int numero_macchine) {
    Stazione *nuova_stazione = (Stazione *) malloc(sizeof(Stazione));
    nuova_stazione->distanza = distanza;
    nuova_stazione->numero_macchine = 0;
    nuova_stazione->limite_macchine = numero_macchine;
    nuova_stazione->macchine = (int *) malloc(numero_macchine * sizeof(int));
    return nuova_stazione;
}

// funzione scambia
void scambia(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// aggiungi_macchina
int aggiungi_macchina(Stazione *stazione, int autonomia_macchina) {
    if (stazione != NULL) {
        if (stazione->numero_macchine == stazione->limite_macchine) {
            return 0;
        }

        int i = stazione->numero_macchine;
        stazione->macchine[i] = autonomia_macchina;
        stazione->numero_macchine++;

        while (i != 0 && stazione->macchine[i] > stazione->macchine[(i - 1) / 2]) {
            scambia(&stazione->macchine[i], &stazione->macchine[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
        return 1;
    } else
        return 0;
}

// riempi stazione
Stazione *riempi_stazione(Stazione *stazione, int *macchine) {
    for (int i = 0; i < stazione->limite_macchine; i++) {
        aggiungi_macchina(stazione, macchine[i]);
    }
    return stazione;
}

// doppia lista per l'autostrada
typedef struct Nodo {
    Stazione *stazione;
    struct Nodo *prossima_stazione;
    struct Nodo *stazione_precedente;
} Autostrada;

void stampa_stazione(Stazione *stazione) {
    printf("Distanza: %d\n", stazione->distanza);
    printf("Numero macchine: %d\n", stazione->numero_macchine);
    printf("Limite macchine: %d\n", stazione->limite_macchine);
    printf("Macchine presenti:\n");
    for (int i = 0; i < stazione->numero_macchine; i++) {
        printf("%d ", stazione->macchine[i]);
    }
    printf("\n");
}

// Funzione per stampare l'autostrada
void stampa_autostrada(Autostrada *autostrada) {
    Autostrada *supporto = autostrada;
    while (supporto != NULL) {
        printf("Stazione:\n");
        stampa_stazione(supporto->stazione);
        printf("\n");
        supporto = supporto->prossima_stazione;
    }
}

// riempi_autostrada
Autostrada *inserisci_in_autostrada(Autostrada *autostrada, Autostrada *stazione) {
    Autostrada *supporto = autostrada;
    Autostrada *aggiornamento;

    if (supporto->stazione->distanza > stazione->stazione->distanza) {
        stazione->prossima_stazione = supporto;
        supporto->stazione_precedente = stazione;
        aggiornamento = stazione;
        return aggiornamento;
    }

    while (supporto->prossima_stazione != NULL &&
           supporto->prossima_stazione->stazione->distanza < stazione->stazione->distanza) {
        supporto = supporto->prossima_stazione;
    }

    stazione->prossima_stazione = supporto->prossima_stazione;
    supporto->prossima_stazione = stazione;
    stazione->stazione_precedente = supporto;
    if (stazione->prossima_stazione != NULL)
        stazione->prossima_stazione->stazione_precedente = stazione;
    aggiornamento = autostrada;
    return aggiornamento;
}

// controlla se c'è già una stazione a quel km
int stazione_presente(Autostrada *autostrada, int distanza) {
    Autostrada *supporto = autostrada;

    while (supporto != NULL) {
        if (supporto->stazione->distanza == distanza)
            return 1;
        supporto = supporto->prossima_stazione;
    }
    return 0;
}

// lega stazione e autostrada
Autostrada *crea_nodo(Stazione *stazione) {
    Autostrada *nodo = (Autostrada *) malloc(sizeof(Autostrada));
    nodo->stazione = stazione;
    nodo->prossima_stazione = NULL;
    nodo->stazione_precedente = NULL;
    return nodo;
}

// aggiungi_stazione
Autostrada *aggiungi_stazione(Autostrada *autostrada, int distanza, int numero_macchine, int *macchine) {
    if (distanza < 0 || numero_stazioni != 0 && stazione_presente(autostrada, distanza)) {
        return NULL;
    }
    Stazione *nuova_stazione = crea_stazione(distanza, numero_macchine);
    nuova_stazione = riempi_stazione(nuova_stazione, macchine);
    if (numero_stazioni == 0) {
        autostrada->stazione = nuova_stazione;
        autostrada->prossima_stazione = NULL;
        autostrada->stazione_precedente = NULL;
        numero_stazioni++;
        return autostrada;
    }
    Autostrada *nodo = crea_nodo(nuova_stazione);
    Autostrada *aggiornamento = inserisci_in_autostrada(autostrada, nodo);
    numero_stazioni++;
    return aggiornamento;
}

// demolisci stazione
Autostrada *demolisci_stazione(Autostrada *autostrada, int distanza) {
    Autostrada *supporto = autostrada;
    Autostrada *aggiornamento;
    if (distanza < 0)
        return NULL;

    while (supporto != NULL) {
        if (supporto->stazione->distanza == distanza) {
            if (supporto->stazione_precedente == NULL && supporto->prossima_stazione == NULL) {
                numero_stazioni--;
                autostrada = NULL;
                return autostrada;
            }
            if (supporto->stazione_precedente == NULL) {
                aggiornamento = supporto->prossima_stazione;
                supporto->prossima_stazione->stazione_precedente = NULL;
                supporto->prossima_stazione = NULL;
                numero_stazioni--;
                free(supporto);
                return aggiornamento;
            }
            if (supporto->prossima_stazione == NULL) {
                supporto->stazione_precedente->prossima_stazione = NULL;
                supporto->stazione_precedente = NULL;
                numero_stazioni--;
                free(supporto);
                return autostrada;
            }
            supporto->prossima_stazione->stazione_precedente = supporto->stazione_precedente;
            supporto->stazione_precedente->prossima_stazione = supporto->prossima_stazione;
            supporto->prossima_stazione = NULL;
            supporto->stazione_precedente = NULL;
            free(supporto);
            return autostrada;
        }
        supporto = supporto->prossima_stazione;
    }
    return NULL;
}

// inserisco nell' heap seguendo le regole
void inserisci_correttamente(Stazione *stazione, int numero_macchine) { // NOLINT
    int genitore = (numero_macchine - 1) / 2;
    if (stazione->macchine[genitore] < stazione->macchine[numero_macchine]) {
        int supporto = stazione->macchine[genitore];
        stazione->macchine[genitore] = stazione->macchine[numero_macchine];
        stazione->macchine[numero_macchine] = supporto;
        inserisci_correttamente(stazione, genitore);
    }
}

// inserisci in stazione (max heap)
void inserisci_in_stazione(Stazione *stazione, int autonomia) { //NOLINT
    int nuova_dimensione = stazione->numero_macchine + 1;
    int *nuova_memoria = realloc(stazione->macchine, nuova_dimensione * sizeof(int));
    if (nuova_memoria != NULL) {
        stazione->macchine = nuova_memoria;
        stazione->macchine[stazione->numero_macchine] = autonomia;
        inserisci_correttamente(stazione, stazione->numero_macchine);
        stazione->numero_macchine++;
    }
}

// aggiungi auto
Autostrada *aggiungi_auto(Autostrada *autostrada, int distanza, int autonomia) {
    Autostrada *supporto = autostrada;

    while (supporto != NULL) {
        if (supporto->stazione->distanza == distanza) {
            inserisci_in_stazione(supporto->stazione, autonomia);
            return autostrada;
        }
        supporto = supporto->prossima_stazione;
    }
    return NULL;
}

// funzione del max heap
void aggiorna_macchine(Stazione *stazione, int i) { //NOLINT
    int maggiore = i;
    int sinistra = 2 * i + 1;
    int destra = 2 * i + 2;

    if (sinistra < stazione->numero_macchine && stazione->macchine[sinistra] > stazione->macchine[maggiore])
        maggiore = sinistra;

    if (destra < stazione->numero_macchine && stazione->macchine[destra] > stazione->macchine[maggiore])
        maggiore = destra;

    if (maggiore != i) {
        scambia(&stazione->macchine[i], &stazione->macchine[maggiore]);
        aggiorna_macchine(stazione, maggiore);
    }
}

// rottama auto
void rottama_macchina(Stazione *stazione, int valore_da_rimuovere) {
    int indice_da_rimuovere = -1;

    for (int i = 0; i < stazione->numero_macchine; i++) {
        if (stazione->macchine[i] == valore_da_rimuovere) {
            indice_da_rimuovere = i;
            break;
        }
    }

    if (indice_da_rimuovere != -1) {
        stazione->macchine[indice_da_rimuovere] = stazione->macchine[stazione->numero_macchine - 1];
        stazione->numero_macchine--;

        aggiorna_macchine(stazione, indice_da_rimuovere);
    }
}

// funzione rottama auto
Autostrada *rottama_auto(Autostrada *autostrada, int distanza, int autonomia) {
    Autostrada *supporto = autostrada;

    while (supporto != NULL) {
        if (supporto->stazione->distanza == distanza) {
            rottama_macchina(supporto->stazione, autonomia);
            return autostrada;
        }
        supporto = supporto->prossima_stazione;
    }
    return NULL;
}

// lista per il percorso
typedef struct Tappa {
    int tappa;
    struct Tappa *prossima_tappa;
} Percorso;

// stampa il percorso
void stampa_percorso(Percorso *autostrada) {
    Percorso *tappa_corrente = autostrada;
    while (tappa_corrente->prossima_tappa != NULL) {
        printf("%d ", tappa_corrente->tappa);
        tappa_corrente = tappa_corrente->prossima_tappa;
    }
    printf("%d", tappa_corrente->tappa);
    printf("\n");
}

Percorso *percorso_finale;

// creazione lista del percorso
Percorso *inizializza_percorso(int tappa) {
    Percorso *nuovo_percorso = (Percorso *) malloc(sizeof(Percorso));
    nuovo_percorso->tappa = tappa;
    nuovo_percorso->prossima_tappa = NULL;
    return nuovo_percorso;
}

// inserisce la tappa nel percorso
Percorso *inserisci_in_percorso(Percorso *autostrada, int tappa) {
    Percorso *nuova_tappa = (Percorso *) malloc(sizeof(Percorso));
    nuova_tappa->tappa = tappa;
    nuova_tappa->prossima_tappa = autostrada;
    return nuova_tappa;
}

Percorso *appendi_a_percorso(Percorso *autostrada, int tappa) {
    Percorso *nuova_tappa = (Percorso *) malloc(sizeof(Percorso));
    nuova_tappa->tappa = tappa;
    nuova_tappa->prossima_tappa = NULL;

    if (autostrada == NULL) {
        return nuova_tappa;
    } else {
        Percorso *tappa_corrente = autostrada;
        while (tappa_corrente->prossima_tappa != NULL) {
            tappa_corrente = tappa_corrente->prossima_tappa;
        }
        tappa_corrente->prossima_tappa = nuova_tappa;
        return autostrada;
    }
}

int partenza_globale;
int arrivo_globale;

// albero di supporto per il pianifica percorso
typedef struct Cella {
    int distanza;
    int autonomia_massima;
    struct Cella *padre;
    struct Cella *figlio;
    struct Cella *fratello;
} Supporto;

// funzione per creare una Cella
Supporto *crea_albero(int distanza, int autonomia) {
    Supporto *nuovo_albero = (Supporto *) malloc(sizeof(Supporto));
    nuovo_albero->distanza = distanza;
    nuovo_albero->autonomia_massima = autonomia;
    nuovo_albero->padre = NULL;
    nuovo_albero->fratello = NULL;
    nuovo_albero->figlio = NULL;
    return nuovo_albero;
}

// inserimento in albero di supporto su un nuovo livello
Supporto *nuovo_livello(Supporto *albero, Supporto *padre, int distanza, int autonomia) {
    Supporto *nuovo_albero = crea_albero(distanza, autonomia);
    nuovo_albero->padre = padre;
    nuovo_albero->figlio = NULL;
    nuovo_albero->fratello = NULL;

    if (padre->figlio == NULL) {
        padre->figlio = nuovo_albero;
    } else {
        Supporto *ultimo_fratello = padre->figlio;
        while (ultimo_fratello->fratello != NULL) {
            ultimo_fratello = ultimo_fratello->fratello;
        }
        ultimo_fratello->fratello = nuovo_albero;
    }

    return albero;
}

// inserimento in albero di supporto stesso livello
Supporto *aggiungi_a_livello(Supporto *albero, Supporto *padre, int distanza, int autonomia, int livello) {
    Supporto *nuovo_nodo = crea_albero(distanza, autonomia);
    Supporto *nodo_da_modificare = albero;

    for (int i = 0; i < livello; i++) {
        if (nodo_da_modificare == NULL) {
            return albero;
        }
        nodo_da_modificare = nodo_da_modificare->figlio;
    }

    if (nodo_da_modificare == NULL) {
        return albero;
    }

    Supporto *ultimo_fratello = nodo_da_modificare;
    while (ultimo_fratello->fratello != NULL) {
        ultimo_fratello = ultimo_fratello->fratello;
    }
    ultimo_fratello->fratello = nuovo_nodo;

    nuovo_nodo->padre = padre;
    nuovo_nodo->fratello = NULL;

    return albero;
}

// stampa supporto
void stampa_supporto(Supporto *nodo, int livello) { //NOLINT
    if (nodo == NULL) {
        return;
    }
    for (int i = 0; i < livello; i++) {
        printf("  ");
    }
    printf("|-- stazione: %d,autonomia: %d\n", nodo->distanza, nodo->autonomia_massima);
    stampa_supporto(nodo->figlio, livello + 1);
    stampa_supporto(nodo->fratello, livello);
}

// pianifica percorso
Percorso *pianifica_percorso(Autostrada *autostrada, int partenza, int arrivo) { //NOLINT
    Supporto *supporto_albero;
    Supporto *indicatore_livello;
    Supporto *supporto_lista;
    Supporto *ultima_stazione_raggiunta;
    Autostrada *supporto_autostrada = autostrada;
    int livello_corrente = 0;
    int livello = 1;
    int stazione_raggiungibile;

    if (partenza_globale < arrivo_globale) {
        // raggiungo la partenza
        while (supporto_autostrada != NULL && supporto_autostrada->stazione->distanza != partenza) {
            supporto_autostrada = supporto_autostrada->prossima_stazione;
        }
        // se ho trovato la partenza costruisco l'albero di supporto e inizializzo gli indicatori
        if (supporto_autostrada->stazione->distanza == partenza) {
            supporto_albero = crea_albero(partenza, massima_autonomia(supporto_autostrada->stazione));
            indicatore_livello = supporto_albero;
            supporto_lista = supporto_albero;
        } else
            return NULL;

        stazione_raggiungibile =
                supporto_autostrada->stazione->distanza + massima_autonomia(supporto_autostrada->stazione);
        supporto_autostrada = supporto_autostrada->prossima_stazione;
        while (supporto_autostrada != NULL) {
            if (stazione_raggiungibile >=
                supporto_autostrada->stazione->distanza) {
                if (livello) {
                    supporto_albero = nuovo_livello(supporto_albero, indicatore_livello,
                                                    supporto_autostrada->stazione->distanza,
                                                    massima_autonomia(supporto_autostrada->stazione));
                    if (supporto_autostrada->stazione->distanza == arrivo) {
                        percorso_finale = inserisci_in_percorso(percorso_finale,
                                                                supporto_albero->padre->distanza);
                        return percorso_finale;
                    }
                    indicatore_livello = indicatore_livello->figlio;
                    supporto_lista = indicatore_livello;
                    ultima_stazione_raggiunta = indicatore_livello;
                    livello_corrente++;
                    livello = 0;
                } else {
                    supporto_albero = aggiungi_a_livello(supporto_albero, indicatore_livello,
                                                         supporto_autostrada->stazione->distanza,
                                                         massima_autonomia(supporto_autostrada->stazione),
                                                         livello_corrente);
                    if (supporto_autostrada->stazione->distanza == arrivo) {
                        while (supporto_lista->padre != NULL) {
                            percorso_finale = inserisci_in_percorso(percorso_finale, supporto_lista->distanza);
                        }
                        return percorso_finale;
                    }
                    ultima_stazione_raggiunta = ultima_stazione_raggiunta->fratello;
                }
            } else {
                stampa_supporto(supporto_albero,0);
                stampa_autostrada(autostrada);
                while (supporto_lista != NULL) {
                    stazione_raggiungibile = supporto_lista->distanza + supporto_lista->autonomia_massima;
                    if (stazione_raggiungibile >= supporto_autostrada->stazione->distanza) {
                        supporto_albero = nuovo_livello(supporto_albero, supporto_lista,
                                                        supporto_autostrada->stazione->distanza,
                                                        massima_autonomia(supporto_autostrada->stazione));
                        indicatore_livello = indicatore_livello->figlio;
                        supporto_lista = indicatore_livello;
                        ultima_stazione_raggiunta = indicatore_livello;
                        if (supporto_autostrada->stazione->distanza == arrivo) {
                            while (ultima_stazione_raggiunta->padre != NULL) {
                                if(ultima_stazione_raggiunta->distanza != arrivo)
                                    percorso_finale = inserisci_in_percorso(percorso_finale,
                                                                        ultima_stazione_raggiunta->distanza);
                                ultima_stazione_raggiunta = ultima_stazione_raggiunta->padre;
                            }
                            percorso_finale = inserisci_in_percorso(percorso_finale,partenza_globale);
                            return percorso_finale;
                        }
                        break;
                    }
                    supporto_lista = supporto_lista->fratello;
                }
                if (supporto_lista == NULL) {
                    return NULL;
                }
                supporto_lista = indicatore_livello;
            }
            supporto_autostrada = supporto_autostrada->prossima_stazione;
        }
    }else{
        supporto_albero = crea_albero(10,10);
        indicatore_livello = supporto_albero;
        supporto_lista = indicatore_livello;
        ultima_stazione_raggiunta = indicatore_livello;
    }
    free(supporto_albero);
    free(supporto_lista);
    free(indicatore_livello);
    free(ultima_stazione_raggiunta);
    return NULL;
}

int main() {
    char istruzione[MAX_ISTRUZIONE];
    Autostrada *autostrada = (Autostrada *) malloc(sizeof(Autostrada));
    Autostrada *supporto = autostrada;
    autostrada->stazione = NULL;
    autostrada->prossima_stazione = NULL;
    autostrada->stazione_precedente = NULL;

    while (scanf("%s", istruzione) != EOF) {
        if (strcmp(istruzione, "aggiungi-stazione") == 0) { //NOLINT
            int distanza;
            if (scanf("%d", &distanza)) { //NOLINT
            }
            int numero_macchine;
            int *macchine;
            if (scanf("%d", &numero_macchine)) { //NOLINT
                macchine = malloc(numero_macchine * sizeof(int));
                for (int i = 0; i < numero_macchine; i++) {
                    if (scanf("%d", &macchine[i])) { //NOLINT
                    }
                }
                supporto = aggiungi_stazione(supporto, distanza, numero_macchine, macchine);
            }
            if (supporto != NULL) {
                printf("aggiunta\n");
                autostrada = supporto;
            } else {
                printf("non aggiunta\n");
                supporto = autostrada;
            }
        } else if (strcmp(istruzione, "demolisci-stazione") == 0) { //NOLINT
            if (numero_stazioni == 0) { printf("non demolita\n"); }
            else {
                int distanza;
                if (scanf("%d", &distanza)) {}//NOLINT
                if (stazione_presente(autostrada, distanza) == 0) {
                    printf("non demolita\n");
                } else {
                    autostrada = demolisci_stazione(autostrada, distanza);
                    if (autostrada != NULL) {
                        printf("demolita\n");
                    } else {
                        if (numero_stazioni == 0)
                            printf("demolita\n");
                        else {
                            autostrada = autostrada;
                            printf("non demolita\n");
                        }
                    }
                }
            }
        } else if (strcmp(istruzione, "aggiungi-auto") == 0) { //NOLINT
            if (numero_stazioni == 0) { printf("non aggiunta\n"); }
            else {
                int distanza;
                int autonomia_auto;
                if (scanf("%d", &distanza)) {} // NOLINT
                if (scanf("%d", &autonomia_auto)) {} // NOLINT
                if (stazione_presente(autostrada, distanza) == 0)
                    printf("non aggiunta\n");
                else {
                    autostrada = aggiungi_auto(autostrada, distanza, autonomia_auto);
                    if (autostrada != NULL) {
                        printf("aggiunta\n");
                    } else {
                        autostrada = autostrada;
                        printf("non aggiunta\n");
                    }
                }
            }
        } else if (strcmp(istruzione, "rottama-auto") == 0) { //NOLINT
            if (numero_stazioni == 0) { printf("non rottamata\n"); }
            else {
                int distanza;
                int autonomia_auto;
                if (scanf("%d", &distanza)) {} // NOLINT
                if (scanf("%d", &autonomia_auto)) {} //NOLINT
                if (stazione_presente(autostrada, distanza) == 0) {
                    printf("non rottamata\n");
                } else {
                    autostrada = rottama_auto(autostrada, distanza, autonomia_auto);
                    if (autostrada != NULL) {
                        printf("rottamata\n");
                    } else {
                        autostrada = autostrada;
                        printf("non rottamata\n");
                    }
                }
            }
        } else if (strcmp(istruzione, "pianifica-percorso") == 0) {  //NOLINT
            free(percorso_finale);
            if (scanf("%d", &partenza_globale)) {} //NOLINT
            if (scanf("%d", &arrivo_globale)) {} //NOLINT
            int partenza = partenza_globale;
            int arrivo = arrivo_globale;
            if (partenza < arrivo)
                percorso_finale = inizializza_percorso(arrivo);
            if (partenza > arrivo)
                percorso_finale = inizializza_percorso(arrivo);
            if (partenza == arrivo)
                percorso_finale = inizializza_percorso(partenza);
            else {
                percorso_finale = pianifica_percorso(autostrada, partenza, arrivo);
            }
            if (percorso_finale != NULL) {
                stampa_percorso(percorso_finale);
            } else {
                printf("nessun percorso\n");
            }
        }
    }
    return 0;
}
// aggiungi-stazione