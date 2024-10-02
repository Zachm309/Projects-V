from flask import Flask, render_template

# Create an instance of the Flask class
app = Flask(__name__)

# Define the route for the root URL
@app.route('/')
def hello_world():
    return render_template('index.html')  # Render the HTML template

# Run the Flask application
if __name__ == '__main__':
    app.run(debug=True)  # Run in debug mode to allow live reloading
