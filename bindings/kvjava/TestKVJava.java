import java.util.HashMap;
import java.util.Map;
import java.lang.String;

public class TestKVJava {

  private static void printMap(HashMap<String,String> m) {
    for (Map.Entry<String, String> entry : m.entrySet()) {
        String key = entry.getKey();
        String val = entry.getValue();
        System.out.println(key + ": " + val);
    }
  }

  public static void main(String args[]) {
    KVJava kv = new KVJava("/tmp/spool");
    HashMap<String,String> h = new HashMap<String,String>();

   /*
    * test the writer 
    */
    h.put("user", "troy");
    h.put("id", "100");
    kv.write(h);
    h.put("user", "bob");
    h.put("id", "101");
    kv.write(h);

   /*
    * test the reader 
    */
    HashMap<String,String> m;
    m = kv.read();
    printMap(m);

    /* test non blocking mode */
    kv.blocking = false;
    m = kv.read();
    if (m != null) printMap(m);
    else System.out.println("non-blocking read: no data");

    m = kv.read();
    if (m != null) printMap(m);
    else System.out.println("non-blocking read: no data");
  }
}
