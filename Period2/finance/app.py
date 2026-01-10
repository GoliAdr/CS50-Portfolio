import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    user_id = session["user_id"] #looks at the current user id

    rows = db.execute( #Looks at all transactions from the user, per symbols equals all shares, only keeps the shares that were not sold
        "SELECT symbol, SUM(shares) AS total_shares "
        "FROM transactions "
        "WHERE user_id = ? "
        "GROUP BY symbol "
        "HAVING total_shares > 0",
        user_id
    )

    positions = []
    portfolio_total = 0 #sum of the value of all shares


    for row in rows:
        symbol = row["symbol"] #name of the share
        shares = row["total_shares"] #how many shares

        quote_data = lookup(symbol) #the values of the share right now
        price = quote_data["price"] #current price
        total = shares * price #value

        portfolio_total += total #sums the total to the value of the portfolio

        positions.append({ #attaches a dict so a list list with numbers and values, in positions
            "symbol": symbol,
            "shares": shares,
            "price": price,
            "total": total
        }
        )


    rows = db.execute("SELECT cash FROM users WHERE id = ?", user_id) #takes the cash value from current user
    cash = rows[0]["cash"] #current cash from the user

    grand_total = portfolio_total + cash #total cash

    return render_template("index.html",
                           positions=positions,         #list of all shares
                           cash=cash,                   #complete cash
                           grand_total=grand_total)     #total of all



@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "GET":
        return render_template("buy.html")
    else:
        symbol = request.form.get("symbol") #Gets the requested symbol
        shares = request.form.get("shares") #States the given amount of shares

        if not symbol:
            return apology("must provide symbol", 400)
        if not shares:
            return apology("must provide shares", 400)


        try:
            shares = int(shares) #shares to int
        except ValueError:
            return apology("shares must be a whole number", 400)

        if shares <=0:
            return apology("shares must be positive", 400)


        quote_data = lookup(symbol) #looks at current stock data
        if quote_data is None:
            return apology("invalid symbol", 400)

        price = quote_data["price"] #Current price per share
        cost = shares * price       #Total cost for this buy

        user_id = session["user_id"] #current user
        rows = db.execute("SELECT cash FROM users WHERE id = ?", user_id) #Gets user cash
        cash = rows[0]["cash"]#Current cash balance

        if cost > cash:
            return apology("insufficient balance", 400)

        db.execute( #Inserts this buy into transactions
            "INSERT INTO transactions (user_id, symbol, shares, price) VALUES (?, ?, ?, ?)",
            user_id,
            quote_data["symbol"],
            shares,
            price
        )
        db.execute( #Updates user cash subtracts cost
            "UPDATE users SET cash = cash - ? WHERE id = ?",
            cost,
            user_id
        )

        return redirect("/") #back to portfolio



@app.route("/history")
@login_required
def history():
    user_id = session["user_id"]

    transactions = db.execute( #Gets all transactions for this user
        "SELECT symbol, shares, price, transacted FROM transactions WHERE user_id = ? ORDER BY transacted DESC",
        user_id
    )
    return render_template("history.html", transactions=transactions) #Sends list to the template


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


#edited
@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    if request.method == "GET":
        return render_template("quote.html")
    else:
        symbol = request.form.get("symbol") #Gets the requested symbol
        if not symbol:
            return apology("Must be a symbol", 400)

        quote_data = lookup(symbol) #Looks up stock data

        if quote_data is None:
            return apology("invalid symbol", 400)

        return render_template("quote.html", quote=quote_data) #Shows result on the same page




@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "GET":
        return render_template("register.html")

    else:
        username = request.form.get("username") #Gets username from form
        password = request.form.get("password") #Gets password from form
        confirmation = request.form.get("confirmation") #Gets confirmation from form
        if not username:
            return apology("Must provide a username", 400)

        if not password:
            return apology("Must provide a password", 400)

        if password != confirmation:
            return apology("Passwords do not match", 400)


        rows = db.execute("SELECT * FROM users WHERE username = ?", username)   #Checks if username exists
        if len(rows) != 0:                                                      #Username already taken
            return apology("username already taken", 400)


        hash = generate_password_hash (password)


        user_id = db.execute(           #Inserts new user into the database
            "INSERT INTO users (username, hash) VALUES (?, ?)",
        username,
        hash
        )


        session["user_id"] = user_id #Logs in the new user

        return redirect("/")



#edited

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    user_id = session["user_id"] #Gets current user id

    if request.method == "GET":
        positions = db.execute( #Gets all symbols where user owns shares
            "SELECT symbol, SUM(shares) AS total_shares FROM transactions " \
            "WHERE user_id = ? Group BY symbol " \
            "HAVING total_shares > 0",
            user_id
        )
        return render_template("sell.html", positions=positions)#Shows sell form


    else:
          symbol = request.form.get("symbol") #Gets selected symbol from the form
          shares = request.form.get("shares") #Gets amount of shares to sell

          if not symbol:
              return apology("must provide symbol", 400)
          if not shares:
              return apology("must provide shares", 400)


          try:
              shares = int(shares)

          except ValueError:
              return apology("shares must be a whole number", 400)

          if shares <=0:
              return apology("shares must be positive", 400)

          rows = db.execute( #Gets how many shares user owns of this symbol
              "SELECT SUM(shares) AS total_shares FROM transactions " \
              "WHERE user_id = ? AND symbol = ? "
              "Group BY symbol",
              user_id,
              symbol
          )

          if len(rows) != 1 or rows[0]["total_shares"] is None: #User owns none of this symbol
              return apology("no shares to sell", 400)

          current_shares = rows[0]["total_shares"] #Current holding for this symbol

          if shares > current_shares:
              return apology("Too many shares", 400)
          quote_data = lookup(symbol)

          if quote_data is None:
              return apology("invalid symbol", 400)

          price = quote_data["price"] #Current price per share
          value = shares * price #Money the user will receive

          db.execute(  #Inserts sale as negative shares
              "INSERT INTO transactions (user_id, symbol, shares, price) "
              "VALUES (?, ?, ?, ?)",
              user_id,
              symbol,
              -shares,  # negative shares für Verkauf
              price
          )

          db.execute( #Adds sale value to user cash
              "UPDATE users SET cash = cash + ? WHERE id = ?",
              value,
              user_id,
          )

          return redirect("/") #back to portfolio
