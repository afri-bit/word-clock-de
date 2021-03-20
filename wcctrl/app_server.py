import os
import logging
import traceback
import pickle

from flask import Flask, render_template, jsonify, request

from wcctrl.config.user import *

app_name = "Word Clock"
app = Flask(__name__, template_folder='../resources/templates', static_folder='../resources/static')

user_config = UserConfig()

shared_memory = "/dev/shm/wc_shm"


def write_shm():
    with open(shared_memory, "wb") as f:
        print(user_config.get_config())
        cfg = user_config.get_config()
        f.write(pickle.dumps(cfg))


def init_shm():
    if not os.path.isfile(shared_memory):
        write_shm()


@app.route("/")
@app.route("/home")
@app.route("/index")
def home():
    return render_template("index.html")


@app.route('/get/config', methods=['GET'])
def get_config():
    return jsonify(user_config.get_config())


@app.route('/get/color/background', methods=['GET'])
def get_color_background():
    return jsonify(user_config.get_color_background().__dict__)


@app.route('/get/color/active_letter', methods=['GET'])
def get_color_active_letter():
    return jsonify(user_config.get_color_active_letter().__dict__)


@app.route('/get/color/minute', methods=['GET'])
def get_color_minute():
    return jsonify(user_config.get_color_minute().__dict__)


@app.route('/get/color/second', methods=['GET'])
def get_color_second():
    return jsonify(user_config.get_color_second().__dict__)


@app.route('/get/color/ampm', methods=['GET'])
def get_color_ampm():
    return jsonify(user_config.get_color_ampm().__dict__)


@app.route('/get/light', methods=['GET'])
def get_light():
    return jsonify(user_config.get_light().__dict__)


@app.route('/get/brightness', methods=['GET'])
def get_brightness():
    return jsonify(user_config.get_brightness().__dict__)


@app.route('/set/color/background', methods=['GET', 'POST'])
def set_color_background():
    try:
        data_dict = json.loads(request.data.decode("utf-8"))
        user_config.set_color_background(ColorRGB(data_dict["red"], data_dict["green"], data_dict["blue"]))
        write_shm()
    except:
        logging.error(traceback.format_exc())
        return jsonify({"status": False})

    return jsonify({"status": True})


@app.route('/set/color/active_letter', methods=['GET', 'POST'])
def set_color_active_letter():
    try:
        data_dict = json.loads(request.data.decode("utf-8"))
        user_config.set_color_active_letter(ColorRGB(data_dict["red"], data_dict["green"], data_dict["blue"]))
        write_shm()
    except:
        logging.error(traceback.format_exc())
        return jsonify({"status": False})

    return jsonify({"status": True})


@app.route('/set/color/minute', methods=['GET', 'POST'])
def set_color_minute():
    try:
        data_dict = json.loads(request.data.decode("utf-8"))
        user_config.set_color_minute(ColorRGB(data_dict["red"], data_dict["green"], data_dict["blue"]))
        write_shm()
    except:
        logging.error(traceback.format_exc())
        return jsonify({"status": False})

    return jsonify({"status": True})


@app.route('/set/color/second', methods=['GET', 'POST'])
def set_color_second():
    try:
        data_dict = json.loads(request.data.decode("utf-8"))
        user_config.set_color_second(ColorRGB(data_dict["red"], data_dict["green"], data_dict["blue"]))
        write_shm()
    except:
        logging.error(traceback.format_exc())
        return jsonify({"status": False})

    return jsonify({"status": True})


@app.route('/set/color/ampm', methods=['GET', 'POST'])
def set_color_ampm():
    try:
        data_dict = json.loads(request.data.decode("utf-8"))
        user_config.set_color_ampm(ColorRGB(data_dict["red"], data_dict["green"], data_dict["blue"]))
        write_shm()
    except:
        logging.error(traceback.format_exc())
        return jsonify({"status": False})

    return jsonify({"status": True})


@app.route('/set/light', methods=['GET', 'POST'])
def set_light():
    try:
        data_dict = json.loads(request.data.decode("utf-8"))
        user_config.set_light(Light(data_dict["always_on"], data_dict["timeout"]))
        write_shm()
    except:
        logging.error(traceback.format_exc())
        return jsonify({"status": False})

    return jsonify({"status": True})


@app.route('/set/brightness', methods=['GET', 'POST'])
def set_brightness():
    try:
        data_dict = json.loads(request.data.decode("utf-8"))
        user_config.set_brightness(Brightness(data_dict["auto"], data_dict["max"], data_dict["value"]))
        write_shm()
    except:
        logging.error(traceback.format_exc())
        return jsonify({"status": False})

    return jsonify({"status": True})


if __name__ == '__main__':
    # Initialize shared memory
    init_shm()

    # Run the server
    app.run(debug=True, host='192.168.1.250')
