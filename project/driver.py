import subprocess;
import re;

import statistics;

import numpy as np;
import matplotlib.pyplot as plt;

def run_child_process(v):
    p1 = subprocess.Popen(["/home/agurfink/ece650/graphGen/graphGen",str(v)],stdout=subprocess.PIPE);
    p2 = subprocess.Popen(["./ece650-prj","-t"],stdin=p1.stdout,stdout=subprocess.PIPE);

    output, _ = p2.communicate();
    return output.decode();

def convert_to_float(lst):
    return [float(_) for _ in lst];



def draw(analysis):
    print("called");
    x = np.array(list(analysis.keys()));
    # mean = np.array([analysis[_][0][0][0] for _ in analysis]);
    # div = np.array([analysis[_][0][0][1] for _ in analysis]);

    # plt.figure(figsize=(8,5));
    # plt.plot(x,mean,color='blue',marker='o',label='CNF');
    # plt.fill_between(x,mean-div,mean+div, color='blue', alpha = 0.1);

    mean = np.array([analysis[_][0][1][0] for _ in analysis]);
    div = np.array([analysis[_][0][1][1] for _ in analysis]);

    plt.plot(x,mean,color='orange',marker='o',label='VC-1');
    plt.fill_between(x,mean-div,mean+div, color='orange', alpha = 0.1);

    mean = np.array([analysis[_][0][2][0] for _ in analysis]);
    div = np.array([analysis[_][0][2][1] for _ in analysis]);

    plt.plot(x,mean,color='red',marker='o',label='VC-2');
    plt.fill_between(x,mean-div,mean+div, color='red', alpha=0.1);


    plt.xlabel('|V|');
    plt.ylabel('Average Running Time (Second)');
    plt.title('Running Time vs Vertex');
    plt.grid();
    plt.legend();

    plt.savefig("Running Time vs vertex.png (approx)",format='png',dpi=300);


def main():
    table = {};
    analysis = {};

    try:
        for v in range(5,51,5):
            table[v] = [];
            analysis[v] = [];

            for _ in range(0,50):
                output = run_child_process(v);
                ans = re.findall(r"( \d+\.?\d*e?-?\d*)", output);
                table[v].append(convert_to_float(ans));
                print(output);
                print(ans);

            print("table = ",table[v]);

            x1 = [_[0] for _ in table[v] if len(_) == 3];
            x2 = [_[1] for _ in table[v] if len(_) == 3];
            x3 = [_[2] for _ in table[v] if len(_) == 3];

            analysis[v].append(
                [(
                    statistics.mean(x1),
                    statistics.stdev(x1)
                ),
                (
                    statistics.mean(x2),
                    statistics.stdev(x2)
                ),
                (
                    statistics.mean(x3),
                    statistics.stdev(x3)
                )]
            );
        else:
            print(analysis);
            draw(analysis=analysis);
    except Exception as e:
        print(f"An error occurred: {e.with_traceback()}");

if __name__ == "__main__":
    main();
