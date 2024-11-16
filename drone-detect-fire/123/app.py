from flask import Flask, render_template, url_for
from flask_mysqldb import MySQL
import os

app = Flask(__name__)

# MySQL configuration (assuming it's already set up)
app.config['MYSQL_HOST'] = '127.0.0.1'
app.config['MYSQL_USER'] = 'root'
app.config['MYSQL_PASSWORD'] = '123456'
app.config['MYSQL_DB'] = 'DRONE'

mysql = MySQL(app)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/header')
def header():
    return render_template('header.html')

@app.route('/left_content')
def left_content():
    return render_template('left-content.html')

@app.route('/identification')
def identification():
    try:
        # Retrieve image path from database
        cur = mysql.connection.cursor()
        cur.execute("SELECT image_path FROM detected ORDER BY ID DESC LIMIT 1")
        image_data = cur.fetchone()
        cur.close()

        if image_data:
            image_path = image_data[0]  # Get image path
            full_image_path = os.path.join('AI_pycode', image_path).replace('\\', '/')
            print(f"Full image path: {full_image_path}")  # Debugging print
        else:
            full_image_path = ''  # Default image path

    except Exception as e:
        print(f"Error: {e}")
        full_image_path = ''  # Default image path

    return render_template('identification.html', image_path=full_image_path)

@app.route('/analysis')
def analysis():
    return render_template('analysis.html')

@app.route('/storage')
def storage():
    return render_template('storage.html')

if __name__ == "__main__":
    app.run(debug=True)
