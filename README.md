# rocnikovy-projekt

ento projekt je **ročníkový projekt**, jehož cílem je vytvořit Discord bota, který umožní uživatelům sázet **fiktivní kredity** na sportovní zápasy.  
Kurzy budou získávány pomocí **web scrapingu** ze sázkových webů (pouze demonstračně).  

Bot kombinuje více oblastí IT:
- práci s **API** (Discord bot),
- **databáze** (SQLite / PostgreSQL),
- **web scraping** (získávání kurzů),
- **herní logiku** (správa sázek, výpočty výher).

---

## 🎯 Cíl projektu
- Naučit se propojit Discord API s databází  
- Implementovat základní ekonomiku (fiktivní kredity, sázky, výhry/prohry)  
- Získávat aktuální kurzy pomocí scraperu  
- Umožnit více uživatelům soutěžit o nejvyšší zůstatek  
- Ukázat komplexní využití Pythonu v praxi  

---

## 🛠️ Použité technologie
- **Python 3.10+**
- [discord.py](https://github.com/Rapptz/discord.py) – knihovna pro Discord API  
- [SQLite3](https://www.sqlite.org/index.html) – databáze pro ukládání uživatelských dat  
- [BeautifulSoup4](https://pypi.org/project/beautifulsoup4/) – scraping kurzů  
- [Requests](https://pypi.org/project/requests/) – získávání HTML stránek  
- Volitelně: **PostgreSQL / MySQL** (pokud bude projekt nasazen na server)  

---

## 📂 Hlavní funkce bota
- Registrace uživatelů a přidělení startovních kreditů  
- Příkaz pro kontrolu zůstatku  
- Sázení na dostupné zápasy s fiktivními kurzy  
- Evidence sázek v databázi  
- Vyhodnocování výsledků a úprava kreditů  
- Leaderboard nejlepších uživatelů  
