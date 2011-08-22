/**
 * Fireworks.cpp
 * Kleine Klasse zum Anzeigen eines Feuerwerks.
 *
 * @mc       Arduino/RBBB
 * @autor    Andreas Muttscheller
 * @version  1.0
 * @datum    18.3.2011
 */
#include "plugin_Fireworks.h"
#include "DisplayMatrix.h"

// Die Feuerwerksstatusdefinitionen
#define NEW     0
#define OFFSET  1
#define FLIGHT  2
#define EXPLODE 3
#define DONE    9

#define MAXCLASSES 3

// H�he und Breite der Matrix
int Fireworks_matrixX;
int Fireworks_matrixY;
int Fireworks_randomClasses;

// Fireworks Klassen
classFirework cFireworks[MAXCLASSES];

boolean Fireworks_alldone;

// Diese Funktionen werden von der QlockThree aufgerufen.
// Nun werden die eigentlichen Funktionen f�r alle Objekte aufgerufen
void initFireworks(int x, int y)
{
	int i;
	
	Fireworks_matrixX = x;
	Fireworks_matrixY = y;
	
	// die Klassen erstellen
	for (i=0;i<MAXCLASSES;i++)
	{
		cFireworks[i].initFireworks(x, y);
	}
	
	Fireworks_alldone = false;
	Fireworks_randomClasses = random(1, MAXCLASSES + 1); // random gibt max-1 zur�ck!
	
	return;
}

void updateFireworks(int timeDiff)
{
	int i;
	// nun f�r jede Klasse die Funktion ausf�hren
	for (i=0;i<Fireworks_randomClasses;i++)
	{
		cFireworks[i].updateFireworks(timeDiff);
	}	
	return;
}

void showFireworks()
{
	int i;
  
  clearMatrix();
  
	// nun f�r jede Klasse die Funktion ausf�hren
	for (i=0;i<Fireworks_randomClasses;i++)
	{
		cFireworks[i].showFireworks();
		Fireworks_alldone = true;
		if (!cFireworks[i].done)
		{
			Fireworks_alldone = false;
		}
	}
	if (Fireworks_alldone == true)
	{
		for (i=0;i<Fireworks_randomClasses;i++)
		{
			cFireworks[i].resetStatus();
		}
    Fireworks_alldone = false;
		
		Fireworks_randomClasses = random(1, MAXCLASSES + 1);
	}
  
  // Die Matrix auf die LEDs multiplexen
  writeMatrix();
  
	return;
}
	
void buttonFireworks(Button btn, byte id)
{

}

//********************************************************************************************
// Classenfunktionen
//********************************************************************************************

void classFirework::initFireworks(int x, int y)
{	
	memset(&firework, 0, sizeof(firework));
	firework.status = NEW;
	done = false;
	return;
}

int classFirework::getX()
{
	return firework.x;
}

void classFirework::resetStatus()
{
  memset(&firework, 0, sizeof(firework));
	firework.status = NEW;
	done = false;
}

boolean classFirework::validateXY(int x, int y)
{
	if (x < 0 || x > (Fireworks_matrixX - 1) || y < 0 || y > (Fireworks_matrixY - 1))
	{
		return false;
	}
	else
	{
		return true;
	}
}

boolean classFirework::getMatrixShow(int x, int y)
{
  if (validateXY(x, y))
  {
    return intMatrix[x][y];
  }
  
  return false;
}

void classFirework::setMatrixShow(int x, int y)
{
  if (validateXY(x, y))
  {
    intMatrix[x][y] = true;
  }
  
  return;
}

//********************************************************************************************
// Update Funktionen
//********************************************************************************************

// Das Feuerwerk initialisieren
void classFirework::initializeFirework()
{
	// wo soll gestartet werden?
	// random(min, max) gibt direkt Maximal "max-1" zur�ck
	// Prüfen, ob eine andere Klasse schon die Spalte nutzt
	int i;
	int classX[Fireworks_randomClasses];
	boolean usable;

        done = false;
	
	// nun f�r jede Klasse die Funktion ausf�hren
	for (i=0;i<Fireworks_randomClasses;i++)
	{
		classX[i] = cFireworks[i].getX();
	}
	
	do
	{
		usable = true;
		firework.x = random(0, Fireworks_matrixX);
		for (i=0;i<Fireworks_randomClasses;i++)
		{
			if (classX[i] == firework.x)
			{
				usable = false;
			}
		}
	} while (usable == false);
	
	
	// Die H�he ermitteln, da die Rakete nicht direkt am Anfang explodieren soll,
	// -4 Rechnen. D.h. die Rakete explodiert erst ab der 3. Reihe
	firework.explosionY = random(0, Fireworks_matrixY - 3);
	
	// die H�he ist am Anfang immer die unterste Zeile
	firework.y = Fireworks_matrixY - 1;
	
	firework.explosionCount  = 0;
        // Radius!
	firework.maxExplosion    = random(2, 4);
  
  // start Offset
  firework.startOffset     = random(1, 4);

	// Status updaten
	firework.status = OFFSET;
	
	return;
}

void classFirework::updateOffset()
{
  firework.startOffset--;
  
  // Ist das Offset erreicht, dann ist der Status FLIGHT
  if (firework.startOffset == 0)
  {
    firework.status = FLIGHT;
  }
  
  return;
}

// Die Flugbahn updaten
void classFirework::updateFlight()
{
	firework.y--;
	
	// Explosion erreicht?
	if (firework.y <= firework.explosionY)
	{
		firework.status = EXPLODE;
	}
	return;
}

// Die Explosion steuern
void classFirework::updateExplosion()
{
	int w;
	int h;
	// interne Matrix initialisieren
  memset(&intMatrix, 0, sizeof(intMatrix));
	
	// Hier muss einfach nur der Count hochgez�hlt werden
	firework.explosionCount++;
	
	if (firework.explosionCount > firework.maxExplosion)
	{
		firework.status = DONE;
	}
	for (w=0;w<=firework.explosionCount;w++)
	{
		for (h=0;h<=firework.explosionCount;h++)
		{
			if (random(0, 2) == 1)
			{
			  setMatrixShow(firework.x + w, firework.y + h);
			}
			if (random(0, 2) == 1)
			{
                          setMatrixShow(firework.x - w, firework.y - h);
			}
			if (random(0, 2) == 1)
			{
                          setMatrixShow(firework.x + w, firework.y - h);
			}
			if (random(0, 2) == 1)
			{
                          setMatrixShow(firework.x - w, firework.y + h);
			}
		}
	}
	return;
}

void classFirework::updateFireworks(int timeDiff)
{
	// je nach status erstmal was anderes machen
	switch (firework.status)
	{
		case NEW:	initializeFirework();
						break;
    case OFFSET:  updateOffset();
            break;
		case FLIGHT:	updateFlight();
						break;
		case EXPLODE:	updateExplosion();
						break;
						// beim DONE wird einmal nichts angezeigt
		case DONE:	done = true;
						break;
                                // Für den Fall, dass was kaputtgegangen ist die Rakete auf DONE
                                // setzen. Später wird es dann alles initialisiert.
                default:        firework.status = DONE;
                                done = true;
                                break;
	}
	return;
}

//********************************************************************************************
// Show Funktionen
//********************************************************************************************
void classFirework::showFlight()
{
	// Die Rakete soll zwei Balken lang sein
	int rocketLen = 2;
	int i = 0;
	
	// Die Rakete zeichen
	for (i=0;i<rocketLen;i++)
	{
		setLED(firework.x, firework.y + i);
	}
	return;
}

void classFirework::showExplosion()
{
	int h = 0;
	int w = 0;
	
	// die Explosion zeichnen
	for (w=0;w<=firework.explosionCount;w++)
	{
		for (h=0;h<=firework.explosionCount;h++)
		{
			if (getMatrixShow(firework.x + w, firework.y + h))
			{
				setLED(firework.x + w, firework.y + h);
			}
			if (getMatrixShow(firework.x - w, firework.y - h))
			{
				setLED(firework.x - w, firework.y - h);
			}
			if (getMatrixShow(firework.x + w, firework.y - h))
			{
				setLED(firework.x + w, firework.y - h);
			}
			if (getMatrixShow(firework.x - w, firework.y + h))
			{
				setLED(firework.x - w, firework.y + h);
			}
		}
	}
	
	return;
}

void classFirework::showFireworks()
{
	switch (firework.status)
	{
	// NEW macht nichts (1 sek Pause zwischen den einzelnen Rakten)
	case NEW:		break;
	case FLIGHT:	showFlight();
					break;
	case EXPLODE:	showExplosion();
					break;
	// beim DONE wird einmal nichts angezeigt (1 sek pause)
	case DONE:		break;
	}
	return;
}
