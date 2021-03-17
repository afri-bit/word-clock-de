from wcctrl.system.config import SystemConfig
from flask import Flask, render_template, jsonify

sys_config = SystemConfig()

app_name = "Word Clock"
app = Flask(__name__, template_folder='../resources/templates', static_folder='../resources/static')


@app.route("/")
@app.route("/home")
@app.route("/index")
def home():
    return render_template("index.html")


@app.route('/get/color/background')
def bla():
    return jsonify(
        username="afrizal herlambang",
        email="g.user.email",
        id="g.user.id"
    )


if __name__ == '__main__':
    # website_url = "wordclock:5000"
    # app.config['SERVER_NAME'] = website_url
    # app.run(debug=True)

    # TODO: Run the wordclock controller

    # TODO: Run the server
    app.run(debug=True, host='localhost')
